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
/// Implement overloaded operator<< for RawAMove
////////////////////////////////////////////////////////////////////////////////

  //! Overload operator<< for RawMove
  file << "void RawAMove::Print(std::ostream& stream) const" << endl;
  file << "{" << endl;
  file << "stream << \"Move: \" << std::endl;" << endl;

  size_t index = 0;
  for(auto it : map_list)
  {
    Map::iterator it2 = it.begin();
    file << "if(moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    for(it2 = it2;it2 != it.end();it2++)
    {
      file << "else if(moves[" << index << "] == " << it2->second << ") stream << \"\\t" << it2->first << "\" << std::endl;" << endl;
    }
    index++;
  }
  file << "}" << endl << endl;
}
