#ifndef LIBGDL_ALGORITHM_MCTSALTERNATE_DEFAULT_NODE_HPP_INCLUDED
#define LIBGDL_ALGORITHM_MCTSALTERNATE_DEFAULT_NODE_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/move_list.hpp>

#include "node.hpp"

namespace libgdl
{
namespace algorithm
{
namespace mcts_alternate
{

template<class ConfigType>
struct DefaultNode : public Node<DefaultNode<ConfigType>>
{
  typedef typename ConfigType::StateType StateType;
  typedef typename ConfigType::MoveType MoveType;

  size_t n_sim = 0;
  size_t* value = 0;
  size_t turn_role;
  StateType state;
  bool is_terminal = false;

  std::list<DefaultNode*> nexts;

  bool isExpanded = false;
  DefaultNode* parent = NULL;

  MoveList<MoveType> legal_moves = MoveList<MoveType>("");
  typename MoveList<MoveType>::iterator legal_it;

  DefaultNode() {}

  inline DefaultNode(StateType& state, ConfigType& config)
    : state(state), config(config)
  {
    value = new size_t[config.RoleSize()];
    for(size_t i = 0;i < config.RoleSize();i++)
      value[i] = 0;

    config.GetStateMachine().GetLegalMoves(state,
                                           legal_moves,
                                           turn_role,
                                           is_terminal);

    if(is_terminal)
    {
      isExpanded = true;
    }
    else legal_it = legal_moves.begin();
  }

  ~DefaultNode()
  {
    delete[] value;
    for(auto it : nexts)
      delete it;
  }

  inline DefaultNode* AddChild()
  {
    StateType c_state = config.GetStateMachine().GetNextState(state, *legal_it);
    legal_it++;
    if(legal_it == legal_moves.end())
      isExpanded = true;

    DefaultNode* out = new DefaultNode(c_state, config);
    out->parent = this;

    nexts.push_back(out);
    return out;
  }

  inline bool IsExpanded() const
  {
    return isExpanded;
  }

  inline double Eval()
  {
    return ((double)value[parent->turn_role]/n_sim) + config.ExplorationParameter() * std::sqrt(std::log(parent->n_sim) / n_sim);
  }

  inline const size_t* Simulate() const
  {
    return config.GetStateMachine().Simulate5(state);
  }

  inline bool IsTerminal() const
  {
    return is_terminal;
  }

  inline size_t TurnRoleID() const
  {
    return turn_role;
  }

  inline DefaultNode* BestChild()
  {
    DefaultNode* max_node = NULL;
    double max_value = 0;

    for(auto it : nexts)
    {
      double uct = it->Eval();
      if(uct > max_value)
      {
        max_value = uct;
        max_node = it;
      }
    }
    return max_node;
  }

  inline DefaultNode* Parent()
  {
    return parent;
  }

  inline void Update(const size_t* goals)
  {
    for(size_t i = 0;i < config.RoleSize();i++)
      value[i] += goals[i];
    n_sim++;
  }

  ConfigType& config;
};

}
}
}


#endif // LIBGDL_ALGORITHM_MCTSALTERNATE_DEFAULT_NODE_HPP_INCLUDED
