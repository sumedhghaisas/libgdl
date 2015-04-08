#include <boost/lockfree/stack.hpp>

namespace libgdl
{

inline const size_t* StateMachine::Simulate3(const AState& s)
{
  AState temp = s.Clone();

  //PrintState(cout, temp);

  //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

  //PrintMoveList(cout, ml);

  //PrintState(cout, initial_pn_top);

  initial_pn.CrystalUpdate_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals, data_init, n_stack, v_stack);

  //std::cout << initial_pn_top << std::endl;

  //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

  bool is_terminal = data_init[initial_pn.terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;

  AMove m("");

  //size_t t;

  while(!is_terminal)
  {
    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //std::cout << ml << std::endl;

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];
    //for(size_t i = 0;i < role_size;i++)
      //m->moves[i] = *initial_pn_legals[i].begin();
    for(size_t i = 0;i < role_size;i++)
    {
      size_t rnd = rand() % initial_pn_legals[i].size();
      auto it = initial_pn_legals[i].begin();
      for(size_t j = 0;j < rnd;j++)
        it++;
      //cout << i << endl;
      m->moves[i] = *it;
    }

    //std::cout << m << std::endl;

    //temp = GetNextState(temp, m);
    //m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
    //size_t start = util::Timer::microtimer();
    initial_pn.CrystalUpdate_input(m, initial_pn_base_move, initial_pn_top, initial_pn_legals, goals, data_init, n_stack, v_stack);
    //stack_time += util::Timer::microtimer() - start;

    //temp = initial_pn_top.Clone();
    temp.Equate(initial_pn_top);

    //std::cout << temp << std::endl;

    //std::cin >> t;

    //std::cin >> t;

    //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
    initial_pn.CrystalUpdate_base(temp, initial_pn_base, initial_pn_top, initial_pn_legals, goals, data_init, n_stack, v_stack);

    is_terminal = data_init[initial_pn.terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);

    //ml = MoveList<AMove>(initial_pn_legals, role_size);

    //std::cout << ml << std::endl;

    //std::cin >> t;
  }

  //PrintState(cout, temp);

  return GetGoals(temp);

   //for(size_t i = 0;i < role_size;i++)
    //cout << goals[i] << endl;

   //return goals;
}

const size_t* StateMachine::Simulate4(const AState& s)
{
  AState temp = s.Clone();

  size_t role_id = 0;

  for(size_t i = 0;i < role_size;i++)
  {
    if(alt_role_masks[i] & temp)
    {
      role_id = i;
      break;
    }
  }

  initial_pn.CrystalUpdate_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals, role_data[role_id], n_stack, v_stack);

  bool is_terminal = role_data[role_id][initial_pn.terminal_crystal_id] & 0x4000;

  AMove m("");

  //size_t t;

  while(!is_terminal)
  {
    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[role_id], role_size);

    //PrintMoveVector(std::cout, ml);

    //std::cin >> t;

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];

    for(size_t i = 0;i < role_size;i++)
    {
      //size_t start = util::Timer::microtimer();
      //size_t rnd = eng();
      //n_count++;
      //stack_time += util::Timer::microtimer() - start;
      size_t rnd = rand() % role_pn_legals[role_id][i].size();
      auto it = role_pn_legals[role_id][i].begin();
      for(size_t j = 0;j < rnd;j++)
        it++;
      //cout << i << endl;
      m->moves[i] = *it;
    }

    //PrintMove(std::cout, m);

    //temp = GetNextState(temp, m);
    initial_pn.CrystalUpdate_input(m, role_pn_base_move[role_id], role_pn_top[role_id], NULL, goals, role_data[role_id], n_stack, v_stack);

    temp.Equate(role_pn_top[role_id]);

    //PrintState(std::cout, temp);

    //std::cin >> t;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //PrintState(cout, alt_role_masks[i]);
      //PrintState(cout, alt_role_masks[i] & temp);

      //if(alt_role_masks[i] & temp)
      //{
        //role_id = i;
        //break;
      //}
    //}
    role_id = ++role_id % role_size;

    //PrintState(cout, role_pn_base[role_id]);

    initial_pn.CrystalUpdate_base(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_legals[role_id], goals, role_data[role_id], n_stack, v_stack);

    is_terminal = role_data[role_id][initial_pn.terminal_crystal_id] & 0x4000;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return GetGoals(temp);
  //GetGoals_m(temp, goals, GetGoals_buff);
  //return goals;
}

inline const size_t* StateMachine::Simulate5(const AState& s)
{
  AState temp = s.Clone();

  //PrintState(cout, temp);

  //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

  //PrintMoveList(cout, ml);

  //PrintState(cout, initial_pn_top);

  initial_pn.CrystalUpdate_base2(temp, initial_pn_base, initial_pn_top, initial_pn_m_arr, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);

  //std::cout << initial_pn_top << std::endl;

  //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

  bool is_terminal = data_init[initial_pn.terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;

  AMove m("");
  size_t* m_arr = m->moves;
  while(!is_terminal)
  {
    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //std::cout << ml << std::endl;

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];
    //for(size_t i = 0;i < role_size;i++)
      //m->moves[i] = *initial_pn_legals[i].begin();
    for(size_t i = 0;i < role_size;i++)
    {
      size_t rnd = (rand() % initial_pn_m_legal_size[i]) + 1;
      size_t index = 0;
      while(true)
      {
        if(initial_pn_m_arr[i][index])
          rnd--;
        if(!rnd)
        {
          //cout << i << endl;
          m_arr[i] = index;
          break;
        }
        index++;
      }
    }

    //temp = GetNextState(temp, m);
    //m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
    //size_t start = util::Timer::microtimer();
    initial_pn.CrystalUpdate_input2(m, initial_pn_base_move, initial_pn_top, initial_pn_m_arr, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);
    //stack_time += util::Timer::microtimer() - start;

    //temp = initial_pn_top.Clone();
    temp.Equate(initial_pn_top);
    //std::cin >> t;

    //std::cin >> t;

    //std::cout << temp << std::endl;

    //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
    initial_pn.CrystalUpdate_base2(temp, initial_pn_base, initial_pn_top, initial_pn_m_arr, initial_pn_m_legal_size, goals, data_init, n_stack, v_stack);

    is_terminal = data_init[initial_pn.terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);

    //ml = MoveList<AMove>(initial_pn_legals, role_size);

    //std::cout << ml << std::endl;

    //std::cin >> t;
  }

  //PrintState(cout, temp);

  return GetGoals(temp);

   //for(size_t i = 0;i < role_size;i++)
    //cout << goals[i] << endl;

   //return goals;
}

const size_t* StateMachine::Simulate6(const AState& s)
{
  AState temp = s.Clone();

  size_t role_id = 0;

  for(size_t i = 0;i < role_size;i++)
  {
    if(alt_role_masks[i] & temp)
    {
      role_id = i;
      break;
    }
  }

  initial_pn.CrystalUpdate_base2(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_m_arr[role_id], role_pn_m_legal_size[role_id], goals, role_data[role_id], n_stack, v_stack);

  bool is_terminal = role_data[role_id][initial_pn.terminal_crystal_id] & 0x4000;

  AMove m("");

  //size_t t;

  while(!is_terminal)
  {
    //MoveVector<AMove> ml = MoveVector<AMove>(role_pn_legals[role_id], role_size);

    //PrintMoveVector(std::cout, ml);

    //std::cin >> t;

    //size_t rnd = rand() % ml.size();

    //MoveList<AMove>::iterator it = ml.begin();
    //for(size_t i = 0;i < rnd;i++)
      //it++;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //m->moves[i] = *initial_pn_legals[i].begin();
    //}

    //m = ml[rnd];

    for(size_t i = 0;i < role_size;i++)
    {
      size_t rnd = (rand() % role_pn_m_legal_size[role_id][i]) + 1;
      size_t index = 0;
      while(true)
      {
        if(role_pn_m_arr[role_id][i][index])
          rnd--;
        if(!rnd)
        {
          //cout << i << endl;
          m->moves[i] = index;
          break;
        }
        index++;
      }
    }

    //PrintMove(std::cout, m);

    //temp = GetNextState(temp, m);
    initial_pn.CrystalUpdate_input2(m, role_pn_base_move[role_id], role_pn_top[role_id], NULL, NULL, goals, role_data[role_id], n_stack, v_stack);

    temp.Equate(role_pn_top[role_id]);

    //PrintState(std::cout, temp);

    //std::cin >> t;

    //for(size_t i = 0;i < role_size;i++)
    //{
      //PrintState(cout, alt_role_masks[i]);
      //PrintState(cout, alt_role_masks[i] & temp);

      //if(alt_role_masks[i] & temp)
      //{
        //role_id = i;
        //break;
      //}
    //}
    role_id = ++role_id % role_size;

    //PrintState(cout, role_pn_base[role_id]);

    initial_pn.CrystalUpdate_base2(temp, role_pn_base[role_id], role_pn_top[role_id], role_pn_m_arr[role_id], role_pn_m_legal_size[role_id], goals, role_data[role_id], n_stack, v_stack);

    is_terminal = role_data[role_id][initial_pn.terminal_crystal_id] & 0x4000;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return GetGoals(temp);
  //GetGoals_m(temp, goals, GetGoals_buff);
  //return goals;
}

}
