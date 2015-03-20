namespace libgdl
{
namespace propnet
{

template<typename StateType>
void PropNet::InitState(StateType& init)
{
  bool temp = true;

  init.Clear();

  init.Get(0, temp);

  for(auto it : init_props)
  {
    init.Set(it, true);
  }
}

template<typename StateType>
void PropNet::PrintState(std::ostream& stream, const StateType& s)
{
  for(size_t i = 0;i < base_nodes.size();i++)
  {
    bool temp = false;
    s.Get(i, temp);
    if(temp)
      stream << base_nodes[i]->Name() << std::endl;
  }
}

template<typename MoveType>
void PropNet::PrintMove(std::ostream& stream, const MoveType& m)
{
  size_t r_id = 0;
  for(auto it : input_nodes)
  {
    for(auto it2 : it)
    {
      bool temp = false;
      m.Get(r_id, it2.first, temp);
      if(temp)
      {
        stream << it2.second->name << std::endl;
        break;
      }
    }
    r_id++;
  }
}

template<typename CType>
void PropNet::PrintMoveCollection(std::ostream& stream, const CType& mc)
{
  for(auto it : mc)
  {
    PrintMove(stream, it);
    stream << std::endl;
  }
}

template<typename StateType>
void PropNet::GetPropNetBaseMask(StateType& s)
{
  s.Clear();
  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      s.Set(it.first);
}

}
}
