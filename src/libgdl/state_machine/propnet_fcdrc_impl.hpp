namespace libgdl
{
namespace state_machine
{

template<class CollectionType>
void PropnetFCDRC::GetLegalMoves(const StateType& s,
                                 CollectionType& collection,
                                 size_t& turn_role,
                                 bool& is_terminal)
{
  initial_pn.Update2(s, *initial_pn_payload2);

  is_terminal = initial_pn_payload2->terminal;
  if(is_terminal)
    return;
  ReasonerType::MoveSet* m_set = initial_pn_payload2->m_set;

  auto* it = new ReasonerType::MoveSet::const_iterator[role_size];
  for(size_t i = 0;i < role_size;i++)
  {
    it[i] = m_set[i].begin();
    if(m_set[i].size() > 1)
      turn_role = i;
  }

  bool isLoop = true;
  while(isLoop)
  {
    AMove m("");
    for(size_t i = 0;i < role_size;i++)
    {
      m->moves[i] = *it[i];
    }
    collection.push_back(m);

    //std::cout << m << std::endl;

    size_t index = 0;
    while(true)
    {
      it[index]++;
      if(it[index] == m_set[index].end())
      {
        if(index == role_size - 1)
        {
          isLoop = false;
          break;
        }
        it[index] = m_set[index].begin();
        index++;
      }
      else break;
    }
  }
  delete[] it;
}

}
}
