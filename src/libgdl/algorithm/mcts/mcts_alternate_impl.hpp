namespace libgdl
{
namespace algorithm
{
namespace mcts_alternate
{

template<class NodeType, class ConfigType>
void Algorithm<NodeType, ConfigType>::Apply(NodeType* current_node,
                                            ConfigType& config,
                                            size_t stop_time)
{
  NodeType* node = current_node;
  while(util::Timer::microtimer() < stop_time)
  {
    node = current_node;
    while(true)
    {
      config.CurrentNode() = node;
      if(node->IsTerminal())
        break;

      if(!node->IsExpanded())
      {
        node = node->AddChild();
        break;
      }

      node = node->BestChild();
    }

    config.NumSimulations()++;
    const size_t* goals = node->Simulate();

    while(node != NULL)
    {
      node->Update(goals);
      node = node->Parent();
    }
  }
}

}
}
}
