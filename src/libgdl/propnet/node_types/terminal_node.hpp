#ifndef LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED

#include "node.hpp"

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct TerminalNode : public Node
{
  TerminalNode(const std::string& name)
    : Node(name, Node::Type::TERMINAL)
  {}

  std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp);
};

}
}
}


#endif // LIBGDL_PROPNET_TERMINAL_NODE_HPP_INCLUDED
