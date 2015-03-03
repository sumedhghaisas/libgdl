/**
 * @file answer.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Answer class.
 */
#include "answer.hpp"

#include <libgdl/reasoners/gdlreasoner/knowledgebase.hpp>

using namespace std;
using namespace libgdl::core;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlreasoner::logicbase;

Answer::Answer(const Type& t,
               const Argument& question,
               const VariableMap& o_v_map,
               const KnowledgeBase& kb,
               const std::set<size_t>& v,
               Answer* ans)
  : kb (kb), question (question), visited(v), o_v_map(o_v_map), t(t)
{
  if(t == CLAUSE)
  {
    extra = NULL;
    isExtra = false;

    m_position = 0;
    m_onAnAnswer = false;
    toDel_f = true;
    toDel_c = true;

    FactMap::const_iterator it1 = kb.GetAllFacts().find(question.value);

    if(it1 != kb.GetAllFacts().end())
    {
      toDel_f = false;
      facts = &(it1->second);
    }
    else facts = new KnowledgeBase::FactList();

    ClauseMap::const_iterator it2 = kb.GetAllClauses().find(question.value);
    if(it2 != kb.GetAllClauses().end())
    {
      clauses = &(it2->second);
      toDel_c = false;
    }
    else clauses = new KnowledgeBase::ClauseList();
  }

  else if(t == OR)
  {
    m_currentAnswer = NULL;
    current_arg = 0;
  }

  else if(t == NOT)
  {
    v_map = o_v_map;
    // TODO : here encode substitution can be removed...
    m_subAnswer = kb.GetAnswer(*question.args[0], v_map, v);
    m_returnedResult = false;
  }

  else if(t == GROUND)
  {
    this->ans = ans;
    isAnswerReturned = false;
  }

  else if(t == DISTINCT)
  {
    v_map = o_v_map;
    m_distinct = !Unify::EquateWithSubstitution(*question.args[0],
                                                *question.args[1],
                                                v_map);
    m_returnedResult = false;
  }

  else if(t == CACHE)
  {
    const tuple<Argument*, list<VariableMap>>& tup = kb.cached_maps.find(question.Hash())->second;

    maps = &get<1>(tup);
    sub_struct = get<0>(tup);
    maps_it = maps->begin();
  }
}

VariableMap AdjustToQuestion(const Argument* arg, const Argument* adj_to,
                             const VariableMap& v_map)
{
  VariableMap out;

  stack<tuple<const Argument*, const Argument*>> s;
  s.emplace(arg, adj_to);

  while(!s.empty())
  {
    auto& tup = s.top();

    const Argument* arg1 = get<0>(tup);
    const Argument* arg2 = get<1>(tup);

    s.pop();

    if(arg1->t == Argument::Var)
    {
      out[arg2] = v_map.find(arg1)->second;
      continue;
    }

    if(arg1->args.size() != arg2->args.size())
    {
      cerr << "In function AdjustToQuestion, number of arguments do not match." << endl;
      cerr << "Improve Argument hash function." << endl;
      exit(1);
    }

    for(size_t i = 0;i < arg1->args.size();i++)
    {
      s.emplace(arg1->args[i], arg2->args[i]);
    }
  }

  return out;
}

Answer::~Answer()
{
  for(std::list<Argument*>::iterator it = to_del.begin();it != to_del.end();
                                                                          it++)
      delete *it;

  if(t == CLAUSE)
  {
    // if stack is yet net empty we have to delete some subresults
    for (deque<SubAnswer>::iterator i = m_subAnswers.begin();
                                                      i!=m_subAnswers.end(); i++)
    {
      delete i->partAnswer;
    }

    if(toDel_f) delete facts;
    if(toDel_c) delete clauses;

    delete extra;
  }

  else if(t == OR)
  {
     delete m_currentAnswer;
  }

  else if(t == NOT)
  {
    delete m_subAnswer;
  }

  else if(t == GROUND)
  {
    delete ans;
  }
}

bool Answer::next()
{
  if(t == CLAUSE)
  {
    // flag that this object is now on an answer
    m_onAnAnswer = true;

    // if position exceeds sum of all facts and clauses then no next result can
    // be found
    if (m_position >= facts->size() + clauses->size())
    {
      // set flag that this object is not on answer anymore
      m_onAnAnswer = false;
      return false;
    }

    // loop from last fact position
    for (; m_position < facts->size(); m_position++)
    {
      if(m_position == 0) fit = facts->begin();
      const Fact& fact = *fit;

      v_map = o_v_map;


      if (Unify::mgu(question, *(fact.arg), v_map))
      {
        m_position++;
        fit++;
        m_isFactAnswer = true;
        return true;
      }
      fit++;
    }

    // after facts, search in clauses
    while (m_position < facts->size() + clauses->size())
    {
      if(m_position == facts->size()) cit = clauses->begin();
      // the clause against we find solutions
      const Clause& clause = *cit;

      if (m_subAnswers.empty())
      {
        const Argument* e_question = &question;
        // check for infinite recursion
        if(visited.find(clause.id) != visited.end())
        {
          if(extra == NULL)
            extra = Unify::GetPartiallySubstitutedArgument(&question,
                                                           o_v_map,
                                                           e_map);
          e_question = extra;
          isExtra = true;
          v_map = VariableMap();
        }
        else v_map = o_v_map;

        if(Unify::mgu(*e_question, *(clause.head), v_map))
        {
          if(clause.premisses.empty())
          {
            // clauses having no tail
            // This is very seldom; but possible, that there is no tail in the clause
            // e.g. in example maze.kif
            m_position++;
            cit++;
            return true;
          }
          // putting sub answers onto the stack
          SubAnswer sanswer;
          sanswer.nextPremiss = clause.premisses.begin();
          sanswer.headMap = v_map;
          std::vector<Argument*>::const_iterator& current = sanswer.nextPremiss;
          std::set<size_t>* t_visited;
          if(!isExtra)
             t_visited = new std::set<size_t>(visited);
          else t_visited = new std::set<size_t>();
          t_visited->insert(clause.id);
          sanswer.partAnswer = kb.GetAnswer(**current,
                                            sanswer.headMap,
                                            *t_visited);
          delete t_visited;
          sanswer.nextPremiss++;
          m_subAnswers.push_back(sanswer);
        }
      }

      while (!m_subAnswers.empty())
      {
        SubAnswer& tail = m_subAnswers.back();
        if (tail.partAnswer->next())
        {
          // Ok lets go further
          if (tail.nextPremiss != clause.premisses.end())
          {
            SubAnswer nextTail;
            nextTail.nextPremiss = tail.nextPremiss;
            nextTail.headMap = tail.partAnswer->GetVariableMap();
            nextTail.partAnswer = kb.GetAnswer(**nextTail.nextPremiss,
                                               nextTail.headMap,
                                               tail.partAnswer->Visited());
            nextTail.nextPremiss++; // shall show onto the next
            m_subAnswers.push_back (nextTail);
          }
          else
          {
            // final solution
            v_map = tail.partAnswer->GetVariableMap();
            if(isExtra)
            {
              Unify::SpecialMapCompression(e_map, v_map, o_v_map);
            }
            return true;
          }
        }
        else
        {
          delete tail.partAnswer;
          m_subAnswers.pop_back();
        }
      }
      m_position++;
      cit++;
      isExtra = false;
    }

    m_onAnAnswer = false;
    return false;
  }

  else if(t == OR)
  {
    // if current answer has next then go to the same and return true
    if (m_currentAnswer != NULL)
    {
      if (m_currentAnswer->next())
      {
        v_map = m_currentAnswer->GetVariableMap();
        return true;
      }
    }

    while (current_arg++ != question.args.size())
    {
      // delete the previous answer
      delete m_currentAnswer;
      // query knowledge base for answer to this token
      m_currentAnswer = kb.GetAnswer(*question.args[current_arg - 1],
                                     o_v_map,
                                     visited);
      // if there exists a valid result update the variable map with the same
      if (m_currentAnswer->next())
      {
        v_map = m_currentAnswer->GetVariableMap();
        return true;
      }
    }
    return false;
  }

  else if(t == NOT)
  {
    if (m_returnedResult) return false;

    m_not = !m_subAnswer->next();
    m_returnedResult = true;
    return m_not;
  }

  else if(t == GROUND)
  {
    // return true if any result found for question
    v_map = o_v_map;
    if(isAnswerReturned == false)
    {
      isAnswerReturned = true;
      bool out = ans->next();
      return out;
    }
    else return false;
  }

  else if(t == DISTINCT)
  {
    if (m_returnedResult) return false;
    m_returnedResult = true;
    return m_distinct;
  }

  else if(t == CACHE)
  {
    if(maps_it != maps->end())
    {
      maps_it++;
      return true;
    }
    return false;
  }

  std::cerr << "Something is gone wrong!!!" << endl;
  return false;
}
