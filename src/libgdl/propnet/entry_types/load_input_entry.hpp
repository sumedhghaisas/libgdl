#ifndef LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace entry_types
{

struct LoadInputEntry : public Entry
{
  LoadInputEntry(size_t id, size_t r_id, size_t in_id)
    : Entry(id), r_id(r_id), in_id(in_id) {}

  void CodeGen(MemoryManager& mm, std::ostream& ss, std::ostream& ss2)
  {
    size_t mem_id = mm.RequestLocation(id);

    ss << "move->Get(" << r_id << ", " << in_id << ", buff[" << mem_id << "]);" << std::endl;
    //ss2 << "s.Get_" << base_id << "(buff[" << mem_id << "]);" << std::endl;
  }

  size_t r_id;
  size_t in_id;
};

}
}
}


#endif // LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED
