#ifndef LIBGDL_PROPNET_NODE_HPP_INCLUDED
#define LIBGDL_PROPNET_NODE_HPP_INCLUDED

#include <list>
#include <string>
#include <tuple>
#include <set>
#include <map>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/a_state.hpp>
#include <libgdl/core/data_types/a_move.hpp>

#include <boost/unordered_set.hpp>

//#define LIBGDL_DFP_TEST

namespace libgdl
{
namespace propnet
{

namespace node_types
{
  struct Node;
}

struct CrystalData
{
  char type;
  size_t id;
  std::list<size_t> out_degree;
  const node_types::Node* node;
};

class PropNet;
class EntryManager;

namespace node_types
{

struct Node
{
  typedef boost::unordered_set<unsigned short> MoveSet;

  enum class Type{BASE, INPUT, VIEW, OR, NEXT, LEGAL, TERMINAL, GOAL, AND, NOT};
  Node(const std::string& name,
       const Type& type)
    : name(name), type(type), isVisited(false), visit_stamp(0), run_stamp(0),
    holding_value(true), mark(0)
  {}

  virtual ~Node() {}

  inline bool IsLegal() const
  {
    if(type == Type::LEGAL)
      return true;
    return false;
  }

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

  virtual bool InitializeValue(const PropNet&, AState& s, MoveSet* m_set, size_t* goals) = 0;

  virtual bool CrystalInitialize(const PropNet& pn, const std::map<const Node*, size_t>& id_map, signed short* data, AState& s, MoveSet* m_set, size_t* goals, std::set<const Node*>& initialized) = 0;

  virtual void Update(bool value, AState& base, AState& top, AMove& m, MoveSet* m_set, size_t* goals) = 0;

  virtual void CrystalUpdate(signed short val, AState& top, signed short& mem, size_t* legal_size, size_t* goals, bool& terminal) const
  {
    std::cout << LOGID << "Unexpected error occured!" << std::endl;
    exit(1);
  }

  virtual void CrystalUpdate(signed short val, AState& top, signed short& mem, MoveSet* m_set, size_t* goals, bool& terminal) const
  {
    std::cout << LOGID << "Unexpected error occured!" << std::endl;
    exit(1);
  }

  virtual Node* GetCopy_only_info() const = 0;

  virtual void RegisterToPropnet(PropNet& pn, Node* to_reg) const = 0;

  size_t Crystallize(std::map<const Node*, size_t>& id_map, std::map<size_t, CrystalData>& data_map, std::map<size_t, size_t>& init_map, size_t& current_index, size_t& current_m_index) const
  {
    if(out_degree.size() > 256)
    {
      std::cout << "Out of bounds while crystallizing." << std::endl;
    }

    auto it = id_map.find(this);
    if(it != id_map.end())
      return it->second;

    CrystalData cry;

    cry.node = this;

    if(type == Type::AND)
      cry.type = 0;
    else if(type == Type::OR || type == Type::VIEW || type == Type::BASE || type == Type::INPUT)
      cry.type = 1;
    else if(type == Type::NOT)
      cry.type = 2;
    else cry.type = 3;

    cry.id = current_index;
    if(type != Type::NEXT)
      init_map[current_index] = current_m_index++;


    if(type == Type::LEGAL)
      std::cout << UName() << " " << current_m_index - 1 << std::endl;

    current_index++;

    id_map[this] = cry.id;

    for(auto it : out_degree)
      cry.out_degree.push_back(it->Crystallize(id_map, data_map, init_map, current_index, current_m_index));

    data_map[cry.id] = cry;

    return cry.id;
  }

  Node* CreateCopy(PropNet& pn, Node* parent, std::map<const Node*, Node*>& node_map) const
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

    if(parent != NULL)
      out->out_degree.push_back(parent);

    for(auto it : in_degree)
      out->in_degree.push_back(it->CreateCopy(pn, out, node_map));

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
      else std::cout << "skvosfowuwbwbvoubwiubwuvbwuvbkvb" << std::endl;
    }
    else if(parent != NULL) RemoveOutDegree(parent);
  }

  Node* Clone(Node* parent, PropNet& pn, std::map<const Node*, Node*>& copy_map) const
  {
    auto it = copy_map.find(this);

    if(it != copy_map.end())
      return it->second;

    Node* out = GetCopy_only_info();

    copy_map[this] = out;

    if(parent != NULL)
      out->out_degree.push_back(parent);

    RegisterToPropnet(pn, out);

    for(auto it : in_degree)
    {
      out->in_degree.push_back(it->Clone(out, pn, copy_map));
    }

    return out;
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

  static size_t node_count;

  size_t mark;
};

}
}
}

#endif // LIBGDL_PROPNET_NODE_HPP_INCLUDED
