#ifndef LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED

#include <list>
#include <tuple>

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace entry_types
{

struct NotEntry : public Entry
{
  NotEntry(size_t id, std::tuple<bool, size_t> in)
    : Entry(id), in(in) {}

  void CodeGen(MemoryManager& mm, std::ostream& ss, std::ostream& ss2)
  {
    size_t ml = mm.RequestLocation(id);

    ss << "buff[" << ml << "] = !";
    PrintAccess(in, mm, ss);
    ss << ";" << std::endl;
  }

  std::tuple<bool, size_t> in;
};

}
}
}

#endif // LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED
