#ifndef LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED

#include <list>
#include <tuple>

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace compilation
{
namespace entry_types
{

struct NotEntry : public Entry
{
  NotEntry(size_t id, std::tuple<bool, size_t> in)
    : Entry(id), in(in) {}

  void CodeGen(MemoryManager& mm, handlers::CodeHandler& ch)
  {
    size_t ml = mm.RequestLocation(id);

    std::stringstream ss;

    ss << "buff[" << ml << "] = ";
    PrintAccess(in, mm, ss);
    ss << ";";

    ch.AddEntry(ss.str());
  }

  std::tuple<bool, size_t> in;
};

}
}
}
}

#endif // LIBGDL_PROPNET_NOT_ENTRY_HPP_INCLUDED
