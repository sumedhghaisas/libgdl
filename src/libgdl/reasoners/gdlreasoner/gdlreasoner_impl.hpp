/**
 * @file gdreasoner_impl.hpp
 * @author Sumedh Ghaisas
 *
 * Implementation of GDL class.
 */
#include <sstream>

namespace libgdl
{
namespace gdlreasoner
{

template<class T>
GDLReasoner::GDLReasoner(T& obj,
                         const Log& log)
  : base_rules(obj, log), log(log)
{
  std::list<Argument*> result = base_rules.Ask("(role ?x)");
  for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    roles.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  result.clear();

  result = base_rules.Ask("(init ?x)");
  std::list<Argument*> temp;
  for(std::list<Argument*>::iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  init = new BState(new core::RawBState(temp));
}

inline bool* GDLReasoner::IsTerminal() const
{
  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::TerminalID;

  bool* out = new bool(base_rules.IsSatisfiable(*question));

  delete question;

  return out;
}

inline MoveList<BMove>* GDLReasoner::GetLegalMoves() const
{
  // get legal moves for all the roles
  std::list<Argument*>* result = new std::list<Argument*>[roles.size()];

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::LegalID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(NULL);
  question->args.push_back(var_x);

  size_t i = 0;
  for(std::list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
  {
    question->args[0] = *it;
    result[i] = base_rules.Ask(*question);
    i++;
  }

  delete var_x;
  question->args.clear();
  delete question;

  std::list<Argument*>::iterator* it = new std::list<Argument*>::iterator[roles.size()];
  for(size_t i = 0;i < roles.size();i++)
    it[i] = result[i].begin();

  MoveList<BMove>* out = new MoveList<BMove>(new core::IntrusiveWrapper<std::list<BMove>>());

  while(true)
  {
    std::vector<Argument*> moves;
    for(size_t i = 0;i < roles.size();i++)
    {
      moves.push_back((*it[i])->args[1]);
    }
    (*out)->push_back(BMove(moves));

    it[0]++;
    size_t index = 1;
    if(it[0] == result[0].end())
    {
      it[0] = result[0].begin();

      while(true)
      {
        if(index == roles.size() ||
          (it[index] == (--result[index].end()) && index == roles.size() - 1))
        {
          for(size_t i = 0;i < roles.size();i++)
          {
            for(std::list<Argument*>::iterator it = result[i].begin();
                                                    it != result[i].end();it++)
            {
              delete *it;
            }
          }

          delete[] result;
          delete[] it;

          return out;
        }
        else if(it[index] == (--result[index].end()))
        {
          it[index] = result[index].begin();
          index++;
        }
        else
        {
          it[index]++;
          break;
        }
      }
    }
  }

  delete[] result;
  delete[] it;

  return out;
}

inline BState* GDLReasoner::GetNextState() const
{
  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::NextID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(var_x);

  // get base propositions true in the next state
  std::list<Argument*> result = base_rules.Ask("(next ?x)");

  delete question;

  std::list<Argument*> temp;
  for(std::list<Argument*>::const_iterator it = result.begin();it != result.end();it++)
  {
    temp.push_back((*it)->args[0]);
    (*it)->args.clear();
    delete *it;
  }
  return new BState(new core::RawBState(temp));
}

inline size_t* GDLReasoner::GetGoal(size_t rid)
{
  std::list<Argument*>::iterator it = roles.begin();
  for(size_t i = 0;i < rid;i++) it++;

  Argument* question = new Argument;
  question->t = Argument::Relation;
  question->value = SymbolTable::GoalID;

  Argument* var_x = new Argument;
  var_x->t = Argument::Var;
  var_x->val = "?x";

  question->args.push_back(*it);
  question->args.push_back(var_x);

  // get goal value for the given role
  std::list<Argument*> result = base_rules.Ask(*question);

  delete var_x;
  question->args.clear();
  delete question;

  std::stringstream stream;
  stream <<
  base_rules.GetSymbolTable()->GetCommandName((*result.begin())->args[1]->value);
  size_t *out = new size_t;
  stream >> *out;

  delete *result.begin();

  return out;
}


inline void GDLReasoner::ApplyState(const BState& state)
{
  const std::list<Argument*>& facts = state.GetProps();
  for(std::list<Argument*>::const_iterator it = facts.begin();it != facts.end();
                                                                          it++)
  {
    Argument *temp = new Argument;
    temp->value = SymbolTable::TrueID;
    temp->t = Argument::Relation;
    temp->args.push_back(*it);

    Fact f;
    f.arg = temp;
    base_rules.m_facts[SymbolTable::TrueID].push_back(std::move(f));
  }
}

inline void GDLReasoner::ApplyActions(const BMove& moves)
{
  size_t r_index = 0;
  for(std::list<Argument*>::const_iterator it = roles.begin();
                                                        it != roles.end();it++)
  {
    Argument *temp = new Argument;
    temp->value = SymbolTable::DoesID;
    temp->t = Argument::Relation;
    temp->args.push_back(*it);
    temp->args.push_back(moves.moves[r_index++]);

    Fact f;
    f.arg = temp;
    base_rules.m_facts[SymbolTable::DoesID].push_back(std::move(f));
  }
}

inline void GDLReasoner::RemoveState()
{
  auto m_it = base_rules.m_facts.find(SymbolTable::TrueID);
  if(m_it != base_rules.m_facts.end())
  {
    KnowledgeBase::FactList& fvec = m_it->second;
    for(auto it = fvec.begin(); it != fvec.end();it++)
    {
      Fact& f = *it;
      f.arg->args.clear();
    }
    base_rules.m_facts.erase(m_it);
  }
}

inline void GDLReasoner::RemoveActions()
{
  KnowledgeBase::FactMap::iterator
                            m_it = base_rules.m_facts.find(SymbolTable::DoesID);
  KnowledgeBase::FactList& fvec2 = m_it->second;
  for(KnowledgeBase::FactList::iterator it = fvec2.begin(); it != fvec2.end();it++)
  {
    Fact& f = *it;
    f.arg->args.clear();
  }
  base_rules.m_facts.erase(m_it);
}

}; // namespace libgdl
}; // namespace gdlreasoner
