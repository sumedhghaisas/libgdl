#ifndef LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace compilation
{
namespace entry_types
{

struct LoadEntry : public Entry
{
  LoadEntry(size_t id, size_t base_id)
    : Entry(id), base_id(base_id) {}

  void CodeGen(MemoryManager& mm, handlers::CodeHandler& ch)
  {
    size_t mem_id = mm.RequestLocation(id);

    std::stringstream ss;

    ss << "s->Get(" << base_id << ", buff[" << mem_id << "]);";

    ch.AddEntry(ss.str());
    //ss2 << "s.Get_" << base_id << "(buff[" << mem_id << "]);" << std::endl;
  }

  size_t base_id;
};

}
}
}
}

#endif // LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED
