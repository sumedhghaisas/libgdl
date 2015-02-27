#include "imove.hpp"

using namespace std;
using namespace libgdl::propnet;

void IMove::CodeGen(std::ostream& file)
{
  //! Add header guards
  file << "#include <libgdl/core/data_types/a_move.hpp>" << endl << endl;

  file << "using namespace std;" << endl;
  file << "using namespace libgdl::core;" << endl;

////////////////////////////////////////////////////////////////////////////////
/// implement static size_t n_roles of RawAMove
////////////////////////////////////////////////////////////////////////////////

  file << "size_t RawAMove::n_roles = " << map_list.size() << ";" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// Generate constructor
////////////////////////////////////////////////////////////////////////////////

  file << "RawAMove::RawAMove(const std::list<std::string>& s_moves)" << endl;
  file << ": moves(new size_t[n_roles]), count(0u)" << endl;
  file << "{" << endl;
  file << "if(s_moves.size() != n_roles)" << endl;
  file << "{" << endl;
  file << "std::cout << \"Incorrect Number of moves provided!\" << std::endl;" << endl;
  file << "exit(1);" << endl;
  file << "}" << endl;

  file << "auto it = s_moves.begin();" << endl;

  size_t index = 0;
  for(auto it : map_list)
  {
    Map::iterator it2 = it.begin();
    file << "if(*it == \"" << it2->first << "\") moves[" << index << "] = " << it2->second << ";" << endl;
    it2++;
    for(it2 = it2;it2 != it.end();it2++)
    {
      file << "else if(*it == \"" << it2->first << "\") moves[" << index << "] = " << it2->second << ";" << endl;
    }
    index++;
    file << "it++;" << endl;
  }

  file << "}" << endl;

////////////////////////////////////////////////////////////////////////////////
/// Implement overloaded operator<< for RawAMove
////////////////////////////////////////////////////////////////////////////////

  //! Overload operator<< for RawMove
  file << "void RawAMove::Print(std::ostream& stream) const" << endl;
  file << "{" << endl;
  file << "stream << \"Move: \" << std::endl;" << endl;

  index = 0;
  for(auto it : map_list)
  {
    Map::iterator it2 = it.begin();
    file << "if(moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    it2++;
    for(it2 = it2;it2 != it.end();it2++)
    {
      file << "else if(moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    }
    index++;
  }
  file << "}" << endl << endl;
}
