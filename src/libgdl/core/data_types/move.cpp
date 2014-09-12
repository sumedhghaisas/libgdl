#include "move.hpp"

#include <sstream>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

Move::Move(const std::string& str,
           SymbolTable& symbol_table,
           Log log)
{
  moves.push_back(new Argument(str, symbol_table, false, log));
  hash = 0;
  for(vector<Argument*>::const_iterator it = moves.begin();it != moves.end();it++)
  {
    size_t temp = (*it)->Hash();
    boost::hash_combine(hash, temp);
  }
}

Move::Move(const std::vector<Argument*>& m)
{
  hash = 0;
  for(vector<Argument*>::const_iterator it = m.begin();it != m.end();it++)
  {
    moves.push_back(new Argument(**it));
    size_t temp = (*it)->Hash();
    boost::hash_combine(hash, temp);
  }
}

Move::Move(const Move& m)
{
  for(size_t i = 0;i < m.moves.size();i++)
    moves.push_back(new Argument(*m.moves[i]));
  hash = m.hash;
}

Move::Move(Move&& m)
{
  moves = m.moves;
  hash = m.hash;
  m.moves.clear();
}

Move::~Move()
{
  for(size_t i = 0;i < moves.size();i++)
    delete moves[i];
  moves.clear();
}

Move& Move::operator=(const Move& m)
{
  for(size_t i = 0;i < moves.size();i++)
    delete moves[i];
  moves.clear();

  for(size_t i = 0;i < m.moves.size();i++)
    moves.push_back(new Argument(*m.moves[i]));
  return *this;
}

std::string Move::DecodeToString(const SymbolTable& symbol_table) const
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

std::string MoveList::DecodeToString(const SymbolTable& symbol_table) const
{
  stringstream s;
  s << "MoveList {" << endl;
  for(libgdl::core::IntrusiveList<libgdl::Move>::const_iterator it = begin();
                                                          it != end();it++)
  {
    s << it->DecodeToString(symbol_table) << std::endl;
  }
  s << "}";
  return s.str();
}
