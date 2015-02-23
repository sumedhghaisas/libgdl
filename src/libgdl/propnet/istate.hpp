#ifndef LIBGDL_PROPNET_ISTATE_HPP_INCLUDED
#define LIBGDL_PROPNET_ISTATE_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>

namespace libgdl
{
namespace propnet
{

struct IState
{
  IState()
    : c_b_no(0) {}

  size_t AddProp(const std::string& str);

  void CodeGen(std::ostream& stream);

  std::map<std::string, size_t> base_m;
  std::vector<std::string> l_base;
  size_t c_b_no;
};

}
}


#endif // LIBGDL_PROPNET_ISTATE_HPP_INCLUDED
