#ifndef LIBGDL_PROPNET_CRYSTAL_DATA_HPP_INCLUDED
#define LIBGDL_PROPNET_CRYSTAL_DATA_HPP_INCLUDED

#include "crystal_config.hpp"

namespace libgdl
{
namespace propnet
{

namespace node_types
{
  struct Node;
}

namespace crystallization
{

struct CrystalData
{
  CrystalConfig::Type type;
  size_t id;
  std::list<size_t> out_degree;
  const node_types::Node* node;
};

}
}
}


#endif // LIBGDL_PROPNET_CRYSTAL_DATA_HPP_INCLUDED
