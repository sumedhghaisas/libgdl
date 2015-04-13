#include <boost/lockfree/stack.hpp>

namespace libgdl
{

inline const size_t* StateMachine::Simulate5(const AState& s)
{
  temp.Equate(s);

  while(!initial_pn.CrystalUpdate_base(temp, *initial_pn_payload))
  {
    initial_pn.GetRandomLegalMove(*initial_pn_payload, m);

    initial_pn.CrystalUpdate_input(m, *initial_pn_payload);

    temp.Equate(initial_pn_payload->GetState());
  }

  return GetGoals(temp);
}

const size_t* StateMachine::Simulate6(const AState& s)
{
  temp.Equate(s);

  size_t role_id = 0;

  for(size_t i = 0;i < role_size;i++)
  {
    if(alt_role_masks[i] & temp)
    {
      role_id = i;
      break;
    }
  }

  while(!initial_pn.CrystalUpdate_base(temp, *role_propnet_payloads[role_id]))
  {
    initial_pn.GetRandomLegalMove(*role_propnet_payloads[role_id], m);

    initial_pn.CrystalUpdate_input(m, *role_propnet_payloads[role_id]);

    temp.Equate(role_propnet_payloads[role_id]->GetState());

    role_id = ++role_id % role_size;
  }

  return GetGoals(temp);
}

//const size_t* StateMachine::Simulate7(const AState& s)
//{
//  AState temp = s.Clone();
//
//  //PrintState(cout, temp);
//
//  //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);
//
//  //PrintMoveList(cout, ml);
//
//  //PrintState(cout, initial_pn_top);
//
//  initial_pn.CrystalUpdate_base(temp, initial_pn_base, initial_pn_top, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);
//
//  //std::cout << initial_pn_top << std::endl;
//
//  //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
//
//  bool is_terminal = data_init[initial_pn.GetCrystalTerminalID()] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;
//
//  AMove m("");
//  size_t* m_arr = m->moves;
//  while(!is_terminal)
//  {
//    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);
//
//    //std::cout << ml << std::endl;
//
//    //size_t rnd = rand() % ml.size();
//
//    //MoveList<AMove>::iterator it = ml.begin();
//    //for(size_t i = 0;i < rnd;i++)
//      //it++;
//
//    //for(size_t i = 0;i < role_size;i++)
//    //{
//      //m->moves[i] = *initial_pn_legals[i].begin();
//    //}
//
//    //m = ml[rnd];
//    //for(size_t i = 0;i < role_size;i++)
//      //m->moves[i] = *initial_pn_legals[i].begin();
//    for(size_t i = 0;i < role_size;i++)
//    {
//      size_t rnd = (rand() % initial_pn_m_legal_size[i]) + 1;
//      size_t index = 0;
//      while(true)
//      {
//        if(data_init[move_crystal_ids[i][index]] & 0x4000)
//          rnd--;
//        if(!rnd)
//        {
//          //cout << i << endl;
//          m_arr[i] = index;
//          break;
//        }
//        index++;
//      }
//    }
//
//    //std::cout << m << std::endl;
//
//    //temp = GetNextState(temp, m);
//    //m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
//    //size_t start = util::Timer::microtimer();
//    initial_pn.CrystalUpdate_input(m, initial_pn_base_move, initial_pn_top, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);
//    //stack_time += util::Timer::microtimer() - start;
//
//    //temp = initial_pn_top.Clone();
//    temp.Equate(initial_pn_top);
//    //std::cin >> t;
//
//    //std::cin >> t;
//
//    //std::cout << temp << std::endl;
//
//    //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
//    initial_pn.CrystalUpdate_base(temp, initial_pn_base, initial_pn_top, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);
//
//    is_terminal = data_init[initial_pn.GetCrystalTerminalID()] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);
//
//    //ml = MoveList<AMove>(initial_pn_legals, role_size);
//
//    //std::cout << ml << std::endl;
//
//    //std::cin >> t;
//  }
//
//  //PrintState(cout, temp);
//
//  return GetGoals(temp);
//}

}
