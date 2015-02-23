#ifndef LIBGDL_PROPNET_HPP_INCLUDED
#define LIBGDL_PROPNET_HPP_INCLUDED

#include <boost/unordered_map.hpp>
#include <map>
#include <vector>
#include <list>

#include <libgdl/core.hpp>
#include <libgdl/reasoners/gdlreasoner/kif_flattener.hpp>
#include <libgdl/gdlparser/kif.hpp>

#include "node_types.hpp"
#include "istate.hpp"
#include "imove.hpp"

namespace libgdl
{
namespace propnet
{

class PropNet
{
  template<typename T1, typename T2>
  using Map = std::map<T1, T2>;

  typedef node_types::Node Node;

 public:
  explicit PropNet(const std::string& filename, Log log = Log());

  explicit PropNet(gdlreasoner::KIFFlattener& kf, Log log = Log());

  ~PropNet();

  bool PrintPropnet(const std::string& filename) const;

  void GenerateStateMachineCode(std::ostream& stream);

  void GenerateMoveCode(std::ostream& stream);

  void GenerateStateCode(std::ostream& stream);

 private:
  void CreatePropNet(gdlreasoner::KIFFlattener& kf);

  node_types::Node* CreateNode(core::SymbolTable sym, const core::Argument* arg);

  std::map<std::string, size_t> roles_ids;

  Map<std::string, Node*> view_nodes;
  Map<size_t, Node*> next_nodes;
  std::vector<Map<size_t, Node*>> input_nodes;
  std::vector<Map<size_t, Node*>> legal_nodes;
  std::vector<Map<size_t, Node*>> goal_nodes;
  Node* terminal;

  Map<size_t, Node*> base_nodes;

  std::list<Node*> and_nodes;
  std::list<Node*> not_nodes;
  std::list<Node*> or_nodes;

  IState istate;
  IMove imove;

  std::list<size_t> init_props;

  size_t c_r_id;
  size_t c_and_id;
  size_t c_not_id;
  size_t c_or_id;
  size_t c_view_id;

  mutable Log log;
};

}
}


#endif // LIBGDL_PROPNET_HPP_INCLUDED
