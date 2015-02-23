#ifndef LIBGDL_PROPNET_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NODE_HPP_INCLUDED

#include <list>
#include <string>
#include <tuple>

namespace libgdl
{
namespace propnet
{

class EntryManager;

namespace node_types
{

struct Node
{
  enum class Type{BASE, INPUT, VIEW, OR, NEXT, LEGAL, TERMINAL, GOAL, AND, NOT};
  Node(const std::string& name,
       const Type& type)
    : name(name), type(type), isVisited(false), visit_stamp(0)
  {}

  virtual ~Node() {}

  void AddIn(Node* n)
  {
    in_degree.push_back(n);
  }

  void AddOut(Node* n)
  {
    out_degree.push_back(n);
  }

  virtual std::string Name() const
  {
    return name;
  }

  virtual std::string UName() const
  {
    std::string out = "";
    if(type == Type::BASE)
    {
      out += "BASE_";
    }

    out += name;
    return out;
  }

  virtual std::tuple<bool, size_t> CodeGen(EntryManager& em, size_t visit_stamp) = 0;

  std::string name;
  std::list<Node*> in_degree;
  std::list<Node*> out_degree;

  Type type;

  bool isVisited;
  std::tuple<bool, size_t> entry_ret;
  size_t visit_stamp;
};

}
}
}

#endif // LIBGDL_PROPNET_NODE_HPP_INCLUDED
