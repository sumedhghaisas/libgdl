#include "move.hpp"

using namespace libgdl;

Move::Move(const std::string& str)
{
  moves.push_back(new Argument(str));
  hash = 0;
  boost::hash_combine(hash, str);
}

Move::Move(const std::vector<Argument*>& m,
       const boost::unordered_map<std::string, size_t>& id_map)
{
  hash = 0;
  for(std::vector<Argument*>::const_iterator it = m.begin();it != m.end();it++)
  {
    moves.push_back(new Argument(**it));
    size_t temp = (*it)->Hash(id_map);
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
