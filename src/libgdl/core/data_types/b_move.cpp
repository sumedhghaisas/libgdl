#include "b_move.hpp"

#include <sstream>

#include <libgdl/core/util/logid.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

BMove::BMove(const std::string& str,
             SymbolTable symbol_table,
             Log log)
{
  moves.push_back(new Argument(str, symbol_table, false, log));
  hash = 0;
  for(vector<Argument*>::const_iterator it = moves.begin();it != moves.end();it++)
  {
    size_t temp = (*it)->Hash(symbol_table, VariableMap());
    boost::hash_combine(hash, temp);
  }
}

BMove::BMove(const std::string& str1,
             const std::string& str2,
             SymbolTable symbol_table,
             Log log)
{
  moves.push_back(new Argument(str1, symbol_table, false, log));
  moves.push_back(new Argument(str2, symbol_table, false, log));
  hash = 0;
  for(vector<Argument*>::const_iterator it = moves.begin();it != moves.end();it++)
  {
    size_t temp = (*it)->Hash(symbol_table, VariableMap());
    boost::hash_combine(hash, temp);
  }
}

BMove::BMove(const std::vector<Argument*>& m)
{
  cout << LOGID << "Not yet implemented!!" << std::endl;
  exit(1);
//  hash = 0;
//  for(vector<Argument*>::const_iterator it = m.begin();it != m.end();it++)
//  {
//    moves.push_back(new Argument(**it));
//    size_t temp = (*it)->Hash(symbol_table, VariableMap());
//    boost::hash_combine(hash, temp);
//  }
}

BMove::BMove(const BMove& m) noexcept
{
  for(size_t i = 0;i < m.moves.size();i++)
    moves.push_back(new Argument(*m.moves[i]));
  hash = m.hash;
}

BMove::BMove(BMove&& m) noexcept
{
  moves = m.moves;
  hash = m.hash;
  m.moves.clear();
}

BMove::~BMove()
{
  for(size_t i = 0;i < moves.size();i++)
    delete moves[i];
  moves.clear();
}

BMove& BMove::operator=(const BMove& m)
{
  for(size_t i = 0;i < moves.size();i++)
    delete moves[i];
  moves.clear();

  for(size_t i = 0;i < m.moves.size();i++)
    moves.push_back(new Argument(*m.moves[i]));
  return *this;
}

std::string BMove::DecodeToString(const SymbolTable& symbol_table) const
{
  stringstream s;
  s << "Move :";
  for(size_t i = 0;i < moves.size();i++)
  {
    s << "\t" << moves[i]->DecodeToString(symbol_table) << endl;
  }
  s << "\tHash = " << std::hex << hash << std::dec;
  return s.str();
}
