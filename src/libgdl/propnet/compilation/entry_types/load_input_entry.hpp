#ifndef LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED
#define LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED

#include "entry.hpp"

namespace libgdl
{
namespace propnet
{
namespace compilation
{
namespace entry_types
{

struct LoadInputEntry : public Entry
{
  LoadInputEntry(size_t id, size_t r_id, size_t in_id)
    : Entry(id), r_id(r_id), in_id(in_id) {}

  void CodeGen(MemoryManager& mm, handlers::CodeHandler& ch)
  {
    size_t mem_id = mm.RequestLocation(id);

    std::stringstream ss;

    ss << "move->Get(" << r_id << ", " << in_id << ", buff[" << mem_id << "]);";

    ch.AddEntry(ss.str());
    //ss2 << "s.Get_" << base_id << "(buff[" << mem_id << "]);" << std::endl;
  }

  size_t r_id;
  size_t in_id;
};

}
}
}
}

#endif // LIBGDL_PROPNET_LOAD_INPUT_ENTRY_HPP_INCLUDED
