#ifndef LIBGDL_PROPNET_OR_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_OR_ENTRY_HPP_INCLUDED

#include <list>
#include <tuple>

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace entry_types
{

struct OrEntry : public Entry
{
  OrEntry(size_t id, const std::list<std::tuple<bool, size_t>>& in_ids)
    : Entry(id), in_ids(in_ids) {}

  void CodeGen(MemoryManager& mm, std::ostream& ss, std::ostream& ss2)
  {
    size_t ml = mm.RequestLocation(id);

    ss << "buff[" << ml << "] = ";
    ss2 << "buff[" << ml << "] = ";

    auto in_id = in_ids.begin();
    PrintAccess(*in_id, mm, ss);
    PrintAccess(*in_id, mm, ss2);
    in_id++;
    for(in_id = in_id;in_id != in_ids.end();in_id++)
    {
      ss << " || ";
      ss2 << " | ";
      PrintAccess(*in_id, mm ,ss);
      PrintAccess(*in_id, mm ,ss2);
    }
    ss << ";" << std::endl;
    ss2 << ";" << std::endl;
  }

  std::list<std::tuple<bool, size_t>> in_ids;
};

}
}
}


#endif // LIBGDL_PROPNET_OR_ENTRY_HPP_INCLUDED