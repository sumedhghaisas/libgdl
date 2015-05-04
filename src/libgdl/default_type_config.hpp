#ifndef LIBGDL_TYPE_CONFIG_HPP_INCLUDED
#define LIBGDL_TYPE_CONFIG_HPP_INCLUDED

#include <libgdl/propnet/propnet.hpp>

namespace libgdl
{

struct DefaultTypeConfig
{
  typedef propnet::PropNet ReasonerType;
  typedef StateMachine<DefaultTypeConfig> StateMachineType;
};

}


#endif // LIBGDL_TYPE_CONFIG_HPP_INCLUDED
