#ifndef LIBGDL_PROPNET_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_ENTRY_HPP_INCLUDED

#include <tuple>
#include <sstream>

#include "../memory_manager.hpp"

#include "../handlers/code_handler.hpp"

namespace libgdl
{
namespace propnet
{
namespace entry_types
{

struct Entry
{
  Entry(size_t id)
    : id(id) {}

  virtual ~Entry() {}

  virtual void CodeGen(MemoryManager& mm, CodeHandler& ch) = 0;

  void PrintAccess(std::tuple<bool, size_t> entry_ret,
                   MemoryManager& mm,
                   std::ostream& ss)
  {
    if(!std::get<0>(entry_ret))
    {
      ss << "!";
    }

    ss << "buff[" << mm.GetLocation(std::get<1>(entry_ret)) << "]";
  }

  size_t id;
};

}
}
}

#endif // LIBGDL_PROPNET_ENTRY_HPP_INCLUDED
