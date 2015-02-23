#ifndef LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace entry_types
{

struct LoadEntry : public Entry
{
  LoadEntry(size_t id, size_t base_id)
    : Entry(id), base_id(base_id) {}

  void CodeGen(MemoryManager& mm, std::ostream& ss, std::ostream& ss2)
  {
    size_t mem_id = mm.RequestLocation(id);

    ss << "s->Get_" << base_id << "(buff[" << mem_id << "]);" << std::endl;
    ss2 << "s.Get_" << base_id << "(buff[" << mem_id << "]);" << std::endl;
  }

  size_t base_id;
};

}
}
}

#endif // LIBGDL_PROPNET_LOAD_ENTRY_HPP_INCLUDED
