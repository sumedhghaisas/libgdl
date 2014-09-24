/**
 * @file knowledgebase.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of KnowledgeBase class.
 */
#include "knowledgebase.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlreasoner::logicbase;
using namespace libgdl::gdlparser;

KnowledgeBase::KnowledgeBase(gdlparser::KIF& kif, const Log& log)
  : c_id(0),
    symbol_table(kif.GetSymbolTable()),
    log(log)
{
  kif.GetSymbolTable() = NULL;

  FactList& facts = kif.Facts();
  ClauseList& clauses = kif.Clauses();

  // import facts from KIF object
  for(FactList::const_iterator it = facts.begin();it != facts.end();it++)
    m_facts[it->arg->value].push_back(std::move(*it));

  // import clauses from KIF object
  for(ClauseList::iterator it = clauses.begin();it != clauses.end();it++)
  {
    it->id = c_id++;
    m_clauses[it->head->value].push_back(std::move(*it));
  }

  kif.Clear();
}

std::list<Argument*> KnowledgeBase::Ask(const Argument& arg,
                                        bool checkForDoubles)
{
  std::list<Argument*> out;

  // get answer
  Answer *ans = GetAnswer(arg, VariableMap(), std::set<size_t>());
  // get all the valid substitution and add them to list

  if(!checkForDoubles)
  {
    while(ans->next())
    {
      Argument* temp = Unify::GetSubstitutedArgument(&arg, ans->GetVariableMap());
      out.push_back(temp);
    }
  }
  else
  {
    std::set<std::string> str_ans;
    while(ans->next())
    {
      Argument* temp = Unify::GetSubstitutedArgument(&arg, ans->GetVariableMap());
      std::stringstream stream;
      stream << *temp;
      if(str_ans.find(stream.str()) == str_ans.end())
      {
        out.push_back(temp);
        str_ans.insert(stream.str());
      }
      else delete temp;
    }
  }
  // delete answer
  delete ans;

  return out;
}

std::list<Argument*> KnowledgeBase::Ask(const std::string& s_arg,
                                        bool checkForDoubles)
{
  Argument arg(s_arg, symbol_table, true, log);

  std::list<Argument*> out;

  // get answer
  Answer *ans = GetAnswer(arg, VariableMap(), std::set<size_t>());
  // get all the valid substitution and add them to list

  if(!checkForDoubles)
  {
    while(ans->next())
    {
      Argument* temp = Unify::GetSubstitutedArgument(&arg, ans->GetVariableMap());
      out.push_back(temp);
    }
  }
  else
  {
    std::set<std::string> str_ans;
    while(ans->next())
    {
      Argument* temp = Unify::GetSubstitutedArgument(&arg, ans->GetVariableMap());
      std::stringstream stream;
      stream << *temp;
      if(str_ans.find(stream.str()) == str_ans.end())
      {
        out.push_back(temp);
        str_ans.insert(stream.str());
      }
      else delete temp;
    }
  }
  // delete answer
  delete ans;

  return out;
}

bool KnowledgeBase::IsSatisfiable(const Argument& arg)
{
  // get answer
  Answer *ans = GetAnswer(arg, VariableMap(), std::set<size_t>());
  // return if any valid substitution exists
  bool res = ans->next();
  delete ans;
  return res;
}

bool KnowledgeBase::IsSatisfiable(const std::string& s_arg)
{
  Argument arg(s_arg, symbol_table, true, log);
  // get answer
  Answer *ans = GetAnswer(arg, VariableMap(), std::set<size_t>());
  // return if any valid substitution exists
  bool res = ans->next();
  delete ans;
  return res;
}

size_t KnowledgeBase::Tell(const Clause& c)
{
  // add the given clause to correct ClauseVec
  ClauseList& cvec = m_clauses[c.head->value];
  cvec.push_back(c);
  cvec.back().id = c_id++;
  // return the index where the clause is added
  return cvec.size() - 1;
}

size_t KnowledgeBase::Tell(Clause&& c)
{
  // add the given clause to correct ClauseVec
  ClauseList& cvec = m_clauses[c.head->value];
  cvec.push_back(std::move(c));
  cvec.back().id = c_id++;
  // return the index where the clause is added
  return cvec.size() - 1;
}

size_t KnowledgeBase::Tell(const Fact& f)
{
  // compute signature and add to appropriate FactVec
  m_facts[f.arg->value].push_back(f);

  // return the index of this fact in the FactVec
  return m_facts[f.arg->value].size() - 1;
}

size_t KnowledgeBase::Tell(Fact&& f)
{
  // compute signature and add to appropriate FactVec
  size_t command = f.arg->value;
  m_facts[command].push_back(std::move(f));

  // return the index of this fact in the FactVec
  return m_facts[command].size() - 1;
}


size_t KnowledgeBase::Tell(const std::string& str)
{
  std::string cmd;
  std::vector<std::string> args;
  if(!Argument::SeparateCommand(str, cmd, args))
  {
    log.Fatal << "Unable to construct argument from " << str << std::endl;
    return 0;
  }

  if(cmd == "<=" && args.size() < 2)
  {
    log.Fatal << "Unable to construct argument from " << str << std::endl;
    return 0;
  }
  else if(cmd != "<=") return Tell(Fact(str, symbol_table, log));

  return Tell(Clause(str, symbol_table, log));
}

bool KnowledgeBase::Erase(const Clause& c, size_t index)
{
  ClauseMap::iterator it;

  // get appropriate ClauseVec to delete from
  it = m_clauses.find(c.head->value);
  if(it == m_clauses.end()) return false;

  ClauseList& cvec = it->second;

  // if index is out of bound then return false
  if(cvec.size() - 1 < index) return false;

  ClauseList::iterator cvec_it = cvec.begin();
  for(size_t i = 0;i < index;i++) cvec_it++;
  cvec.erase(cvec_it);
  if(cvec.size() == 0) m_clauses.erase(m_clauses.find(c.head->value));
  return true;
}

bool KnowledgeBase::Erase(const Fact& f, size_t index)
{
  FactMap::iterator it;

  it = m_facts.find(f.arg->value);
  if(it == m_facts.end()) return false;

  FactList& fvec = it->second;

  if(fvec.size() - 1 < index) return false;

  FactList::iterator fvec_it = fvec.begin();
  for(size_t i = 0;i < index;i++) fvec_it++;
  fvec.erase(fvec_it);
  if(fvec.size() == 0) m_facts.erase(m_facts.find(f.arg->value));
  return true;
}

const KnowledgeBase::FactList*
                        KnowledgeBase::GetFacts(size_t sig) const
{
  FactMap::const_iterator it = m_facts.find(sig);
  if(it == m_facts.end()) return NULL;
  else return &(it->second);
}

const KnowledgeBase::ClauseList*
                      KnowledgeBase::GetClauses(size_t sig) const
{
  ClauseMap::const_iterator it = m_clauses.find(sig);
  if(it == m_clauses.end()) return NULL;
    else return &(it->second);
}

Answer* KnowledgeBase::GetAnswer(const Argument& question,
                                 const VariableMap& v_map,
                                 const std::set<size_t>& visited) const
{
  Answer *ans = NULL;

  if(Unify::IsGroundQuestion(&question, v_map))
  {
    if(question.value == SymbolTable::OrID)
      ans = new GroundQuestionAnswer
                          (new OrClauseAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else if(question.value == SymbolTable::DistinctID)
      ans = new GroundQuestionAnswer
                          (new DistinctAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else if(question.value == SymbolTable::NotID)
      ans = new GroundQuestionAnswer
                          (new NotAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else ans = new GroundQuestionAnswer
                          (new ClauseAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
  }
  else
  {
    if(question.value == SymbolTable::OrID)
      ans = new OrClauseAnswer(question, v_map, *this, visited);
    else if(question.value == SymbolTable::DistinctID)
      ans = new DistinctAnswer(question, v_map, *this, visited);
    else if(question.value == SymbolTable::NotID)
      ans = new NotAnswer(question, v_map, *this, visited);
    else ans = new ClauseAnswer(question, v_map, *this, visited);
  }

  return new AnswerDecoder(ans, question, v_map, *this);
}

std::ostream& operator<<(std::ostream& stream, const KnowledgeBase& kb)
{
  SymbolDecodeStream o(&kb.GetSymbolTable(), stream);

  const KnowledgeBase::FactMap& all_facts = kb.GetAllFacts();
  const KnowledgeBase::ClauseMap& all_clauses = kb.GetAllClauses();

  o << "Facts: " << std::endl;
  for(KnowledgeBase::FactMap::const_iterator it = all_facts.begin();
                                                it != all_facts.end();it++)
  {
    o << it->first << " {" << std::endl;
    const KnowledgeBase::FactList& facts = it->second;
    for(list<Fact>::const_iterator it = facts.begin();it != facts.end();it++)
      o << *it << std::endl;
    o << "}" << std::endl;
  }

  o << std::endl;

  o << "Clauses: " << std::endl;
  for(KnowledgeBase::ClauseMap::const_iterator it = all_clauses.begin();
                                                it != all_clauses.end();it++)
  {
    o << it->first << " {" << std::endl;
    const KnowledgeBase::ClauseList& clauses = it->second;
    for(list<Clause>::const_iterator it2 = clauses.begin();
                                                    it2 != clauses.end();it2++)
      o << *it2 << " [" << it2->id << "]" << std::endl;
    o << "}" << std::endl;
  }

  return stream;
}
