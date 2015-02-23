#include "not_node.hpp"

#include "../entry_manager.hpp"

using namespace std;
using namespace libgdl::propnet;
using namespace libgdl::propnet::node_types;
using namespace libgdl::propnet::entry_types;

tuple<bool, size_t> NotNode::CodeGen(EntryManager& em, size_t v_stamp)
{
  if(visit_stamp != v_stamp)
  {
    visit_stamp = v_stamp;
    isVisited = false;
  }

  if(!isVisited)
  {
    entry_ret = (*in_degree.begin())->CodeGen(em, v_stamp);
    get<0>(entry_ret) = !get<0>(entry_ret);
    isVisited = true;
  }

  return entry_ret;
}
