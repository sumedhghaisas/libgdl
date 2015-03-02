#include "not_node.hpp"

#include "../entry_manager.hpp"
#include "../entry_types/not_entry.hpp"

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
    size_t out = em.GetNewID();

    tuple<bool, size_t> in = (*in_degree.begin())->CodeGen(em, v_stamp);
    em.AddStamp(get<1>(in), out);

    em.AddEntry(new NotEntry(out, in));

    entry_ret = tuple<bool, size_t>(true, out);

    isVisited = true;
  }

  return entry_ret;
}
