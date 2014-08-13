/**
 * @file knowledgebase.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of KnowledgeBase class.
 */
#include "knowledgebase.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::gdlreasoner;
using namespace libgdl::gdlreasoner::logicbase;
using namespace libgdl::gdlparser;

KnowledgeBase::KnowledgeBase(gdlparser::KIF& kif)
  : c_id(0), isDebuggingSymbols(kif.DebuggingSymbolSupport())
{
  std::vector<Fact>& facts = kif.Facts();
  std::vector<Clause>& clauses = kif.Clauses();

  // import facts from KIF object
  for(size_t i = 0; i < facts.size(); i++)
  {
    std::stringstream stream;
    stream << facts[i].arg->args.size();
    std::string command = facts[i].Command() + "/" + stream.str();
    m_facts[command].push_back(std::move(facts[i]));
  }

  // import clauses from KIF object
  for(size_t i = 0; i < clauses.size(); i++)
  {
    std::stringstream stream;
    stream << clauses[i].head->args.size();
    std::string command = clauses[i].head->val + "/" + stream.str();
    m_clauses[command].push_back(std::move(clauses[i]));
  }

  kif.Clear();
}

std::list<Argument*> KnowledgeBase::Ask(const Argument& arg,
                                        bool checkForDoubles) const
{
  std::list<Argument*> out;

  // get answer
  Answer *ans = GetAnswer(arg, Unify::VariableMap(), std::set<size_t>());
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

bool KnowledgeBase::IsSatisfiable(const Argument& arg) const
{
  // get answer
  Answer *ans = GetAnswer(arg, Unify::VariableMap(), std::set<size_t>());
  // return if any valid substitution exists
  bool res = ans->next();
  delete ans;
  return res;
}

size_t KnowledgeBase::Tell(const Clause& c)
{
  // add the given clause to correct ClauseVec
  std::stringstream stream;
  stream << c.head->args.size();
  std::string command = c.head->val + "/" + stream.str();
  ClauseVec& cvec = m_clauses[command];
  cvec.push_back(c);
  cvec.back().id = c_id++;
  // return the index where the clause is added
  return cvec.size() - 1;
}

size_t KnowledgeBase::Tell(const Fact& f)
{
  // compute signature and add to appropriate FactVec
  std::stringstream stream;
  stream << f.arg->args.size();
  std::string command = f.arg->val + "/" + stream.str();
  m_facts[command].push_back(f);

  // return the index of this fact in the FactVec
  return m_facts[command].size() - 1;
}

size_t KnowledgeBase::Tell(const std::string& str)
{
  if(str[0] != '(') return Tell(Fact(str));

  std::string cmd;
  std::vector<std::string> args;
  if(!Argument::SeparateCommand(str, cmd, args))
  {
    std::cerr << "Unable to construct argument from " << str << std::endl;
    return 0;
  }

  if(cmd == "<=" && args.size() < 2)
  {
    std::cerr << "Unable to construct argument from " << str << std::endl;
    return 0;
  }
  else if(cmd != "<=") return Tell(Fact(str));

  return Tell(Clause(str));
}

bool KnowledgeBase::Erase(const Clause& c, size_t index)
{
  std::map<std::string, ClauseVec>::iterator it;

  // get appropriate ClauseVec to delete from
  std::stringstream stream;
  stream << c.head->args.size();
  std::string command = c.head->val + "/" + stream.str();
  it = m_clauses.find(command);
  if(it == m_clauses.end()) return false;

  ClauseVec& cvec = it->second;

  // if index is out of bound then return false
  if(cvec.size() - 1 < index) return false;

  std::list<Clause>::iterator cvec_it = cvec.begin();
  for(size_t i = 0;i < index;i++) cvec_it++;
  cvec.erase(cvec_it);
  if(cvec.size() == 0) m_clauses.erase(m_clauses.find(command));
  return true;
}

bool KnowledgeBase::Erase(const Fact& f, size_t index)
{
  std::map<std::string, FactVec>::iterator it;

  std::stringstream stream;
  stream << f.arg->args.size();
  std::string command = f.arg->val + "/" + stream.str();
  it = m_facts.find(command);
  if(it == m_facts.end()) return false;

  FactVec& fvec = it->second;

  if(fvec.size() - 1 < index) return false;

  std::list<Fact>::iterator fvec_it = fvec.begin();
  for(size_t i = 0;i < index;i++) fvec_it++;
  fvec.erase(fvec_it);
  if(fvec.size() == 0) m_facts.erase(m_facts.find(command));
  return true;
}

const KnowledgeBase::FactVec*
                        KnowledgeBase::GetFacts(const std::string& sig) const
{
  std::map<std::string, FactVec>::const_iterator it = m_facts.find(sig);
  if(it == m_facts.end()) return NULL;
  else return &(it->second);
}

const KnowledgeBase::ClauseVec*
                      KnowledgeBase::GetClauses(const std::string& sig) const
{
  std::map<std::string, ClauseVec>::const_iterator it = m_clauses.find(sig);
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
    if(question.val == "or")
      ans = new GroundQuestionAnswer
                          (new OrClauseAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else if(question.val == "distinct")
      ans = new GroundQuestionAnswer
                          (new DistinctAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else if(question.val == "not")
      ans = new GroundQuestionAnswer
                          (new NotAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
    else ans = new GroundQuestionAnswer
                          (new ClauseAnswer(question, v_map, *this, visited),
                           question, v_map, *this, visited);
  }
  else
  {
    if(question.val == "or")
      ans = new OrClauseAnswer(question, v_map, *this, visited);
    else if(question.val == "distinct")
      ans = new DistinctAnswer(question, v_map, *this, visited);
    else if(question.val == "not")
      ans = new NotAnswer(question, v_map, *this, visited);
    else ans = new ClauseAnswer(question, v_map, *this, visited);
  }

  return new AnswerDecoder(ans, question, v_map, *this);
}

std::ostream& operator<<(std::ostream& o, const KnowledgeBase& kb)
{
  const KnowledgeBase::FactMap& all_facts = kb.GetAllFacts();
  const KnowledgeBase::ClauseMap& all_clauses = kb.GetAllClauses();

  o << "Facts: " << std::endl;
  for(KnowledgeBase::FactMap::const_iterator it = all_facts.begin();
                                                it != all_facts.end();it++)
  {
    o << it->first << " {" << std::endl;
    const KnowledgeBase::FactVec& facts = it->second;
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
    const KnowledgeBase::ClauseVec& clauses = it->second;
    for(list<Clause>::const_iterator it = clauses.begin();it != clauses.end();it++)
      o << *it << std::endl;
    o << "}" << std::endl;
  }

  return o;
}
