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

  void CodeGen(MemoryManager& mm, CodeHandler& ch)
  {
    size_t ml = mm.RequestLocation(id);

    std::stringstream* ss = new std::stringstream();

    *ss << "buff[" << ml << "] = ";

    auto in_id = in_ids.begin();
    PrintAccess(*in_id, mm, *ss);
    in_id++;

    size_t or_count = 0;

    for(in_id = in_id;in_id != in_ids.end();in_id++)
    {
      if(or_count > or_limit)
      {
        *ss << ";";
        ch.AddEntry(ss->str());
        delete ss;
        ss = new std::stringstream();
        *ss << "buff[" << ml << "] = buff[" << ml << "]";
        or_count = or_count - 1;
      }

      *ss << " || ";
      PrintAccess(*in_id, mm , *ss);
      or_count++;
    }

    *ss << ";";

    ch.AddEntry(ss->str());
  }

  std::list<std::tuple<bool, size_t>> in_ids;

  static const size_t or_limit = 20;
};

}
}
}


#endif // LIBGDL_PROPNET_OR_ENTRY_HPP_INCLUDED
