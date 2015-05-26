#include "node.hpp"

using namespace libgdl;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;

size_t Node::Crystallize(std::map<const Node*, size_t>& id_map,
                         std::map<size_t, CrystalData>& data_map,
                         std::map<size_t, size_t>& init_map,
                         size_t& current_index,
                         size_t& current_m_index) const
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

  cry.type = GetCrystalType();
//  if(type == Type::AND)
//    cry.type = CrystalConfig::Type::AND;
//  else if(type == Type::OR || type == Type::VIEW || type == Type::BASE || type == Type::INPUT)
//    cry.type = CrystalConfig::Type::OR;
//  else if(type == Type::NOT)
//    cry.type = CrystalConfig::Type::NOT;
//  else cry.type = CrystalConfig::Type::OR_UPDATE;

  cry.id = current_index;
  init_map[current_index] = current_m_index++;

  current_index++;

  id_map[this] = cry.id;

  for(auto it : out_degree)
    cry.out_degree.push_back(it->Crystallize(id_map, data_map, init_map, current_index, current_m_index));

  data_map[cry.id] = cry;

  return cry.id;
}

Node* Node::CreateCopy(PropNet& pn,
                       Node* parent,
                       std::map<const Node*, Node*>& node_map) const
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

void Node::DFSMark(size_t cm)
{
  if(mark == cm)
    return;

  for(auto it : in_degree)
    it->DFSMark(cm);

  mark = cm;
}

void Node::RemoveOutDegree(Node* out)
{
  for(auto it = out_degree.begin();it != out_degree.end();it++)
    if(*it == out)
    {
      out_degree.erase(it);
      return;
    }
}

void Node::RemoveInDegree(Node* out)
{
  for(auto it = in_degree.begin();it != in_degree.end();it++)
    if(*it == out)
    {
      in_degree.erase(it);
      return;
    }
}

void Node::DeleteIfNotMarked(Node* parent, std::set<Node*>& del, size_t cm)
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

Node* Node::Clone(Node* parent,
                  PropNet& pn,
                  std::map<const Node*, Node*>& copy_map) const
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
