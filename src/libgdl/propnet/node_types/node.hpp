#ifndef LIBGDL_PROPNET_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NODE_HPP_INCLUDED

#include <list>
#include <string>
#include <tuple>
#include <set>
#include <map>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/a_state.hpp>

//#define LIBGDL_DFP_TEST

namespace libgdl
{
namespace propnet
{

class PropNet;
class EntryManager;

namespace node_types
{

struct Node
{
  enum class Type{BASE, INPUT, VIEW, OR, NEXT, LEGAL, TERMINAL, GOAL, AND, NOT};
  Node(const std::string& name,
       const Type& type)
    : name(name), type(type), isVisited(false), visit_stamp(0), run_stamp(0),
    holding_value(true), mark(0)
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

  //virtual Update(bool value) = 0;

  virtual bool InitializeValue(const PropNet&, AState& s, std::set<size_t>* m_set, size_t* goals) = 0;

  virtual void Update(bool value, AState& base, AState& top, AMove& m, std::set<size_t>* m_set, size_t* goals) = 0;

  virtual Node* GetCopy_only_info() = 0;

  virtual void RegisterToPropnet(PropNet& pn, Node* to_reg) = 0;

  Node* CreateCopy(PropNet& pn, Node* parent, std::map<Node*, Node*>& node_map)
  {
    auto nm_it = node_map.find(this);
    if(nm_it != node_map.end())
    {
      Node* out = nm_it->second;
      if(parent != NULL)
        out->out_degree.push_back(parent);
      return out;
    }

    Node* out = GetCopy_only_info();

    RegisterToPropnet(pn, out);

    node_map[this] = out;
    out->out_degree.push_back(parent);

    for(auto it : in_degree)
      out->in_degree.push_back(it->CreateCopy(pn, this, node_map));

    return out;
  }

  void DFSMark(size_t cm)
  {
    if(mark == cm)
      return;

    for(auto it : in_degree)
      it->DFSMark(cm);

    mark = cm;
  }

  void RemoveOutDegree(Node* out)
  {
    for(auto it = out_degree.begin();it != out_degree.end();it++)
      if(*it == out)
      {
        out_degree.erase(it);
        return;
      }
  }

  void DeleteIfNotMarked(Node* parent, std::set<Node*>& del, size_t cm)
  {
    if(mark != cm)
    {
      for(auto it : in_degree)
      {
        if(del.find(it) == del.end())
          it->DeleteIfNotMarked(this, del, cm);
      }

      if(type != Type::BASE && type != Type::INPUT)
      {
        del.insert(this);
        delete this;
      }
      else if(parent != NULL)
      {
        RemoveOutDegree(parent);
      }
    }
    else if(parent != NULL) RemoveOutDegree(parent);
  }

  std::string name;
  std::list<Node*> in_degree;
  std::list<Node*> out_degree;

  Type type;

  bool isVisited;
  std::tuple<bool, size_t> entry_ret;
  size_t visit_stamp;

  size_t run_stamp;
  bool holding_value;

  size_t mark;
};

}
}
}

#endif // LIBGDL_PROPNET_NODE_HPP_INCLUDED
