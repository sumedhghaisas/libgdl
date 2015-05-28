#ifndef LIBGDL_PROPNET_SAVE_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_SAVE_ENTRY_HPP_INCLUDED

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace compilation
{
namespace entry_types
{

struct SaveEntry : public Entry
{
  SaveEntry(size_t id, size_t base_id, const std::list<std::tuple<bool, size_t>>& in_ids)
    : Entry(id), base_id(base_id), in_ids(in_ids) {}

  void CodeGen(MemoryManager& mm, handlers::CodeHandler& ch)
  {
    std::stringstream ss, ss2;

    ss << "s_out->Set(" << base_id << ", ";
    ss2 << "s_out.Set(" << base_id << ", ";

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

    ss << ");";
    ss2 << ");";

    ch.AddEntry(ss.str());
  }

  size_t base_id;
  std::list<std::tuple<bool, size_t>> in_ids;
};

}
}
}
}

#endif // LIBGDL_PROPNET_SAVE_ENTRY_HPP_INCLUDED
