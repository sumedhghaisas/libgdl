#include "istate.hpp"

#include <iostream>
#include <cmath>

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;

size_t IState::AddProp(const string& str)
{
  auto it = base_m.find(str);
  if(it == base_m.end())
  {
    base_m[str] = c_b_no++;
    l_base.push_back(str);
    return c_b_no - 1;
  }

  return it->second;
}

void IState::CodeGen(ostream& file)
{
  size_t size_char = l_base.size() / 8;
  if(l_base.size() % 8 != 0)
  {
    size_char++;
  }

  file << "#include <libgdl/core/data_types/a_state.hpp>" << endl << endl;

  file << "using namespace std;" << endl;
  file << "using namespace libgdl;" << endl;
  file << "using namespace libgdl::core;" << endl << endl;

////////////////////////////////////////////////////////////////////////////////
/// Implement static size_t arr_size of RawAState
////////////////////////////////////////////////////////////////////////////////

  file << "size_t RawAState::arr_size = " << size_char << ";" << endl;

  //! overload operator<< for RawState
  file << "extern \"C\" void PrintAState(ostream& stream, const AState& s) {" << endl;
  file << "bool temp = false;" << endl;
  file << "stream << \"State: \" << std::endl;" << endl;
  for(size_t i = 0;i < l_base.size();i++)
  {
    file << "s->Get(" << i << ", temp);" << endl;
    file << "if(temp) stream << \"\\t\" << \"" << l_base[i] << "\" << std::endl;" << endl;
  }
  file << "}" << endl;
}
