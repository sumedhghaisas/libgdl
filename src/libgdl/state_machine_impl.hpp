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

  UpdateCrystal_base(temp, initial_pn_base_mask, initial_pn_top, initial_pn_base, initial_pn_legals, goals);

  //PrintState(cout, initial_pn_top);

  //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);

  bool is_terminal = initial_pn.data_init[terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;

  AMove m("");

  while(!is_terminal)
  {
    //MoveList<AMove> ml = MoveList<AMove>(initial_pn_legals, role_size);

    //PrintMoveList(cout, ml);

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

    //PrintMove(cout, m);

    //temp = GetNextState(temp, m);
    //m.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_move, initial_pn_input_nodes, NULL, NULL);
    UpdateCrystal_move(m, initial_pn_base_move, initial_pn_top, initial_pn_legals, goals);

    //temp = initial_pn_top.Clone();
    temp.Equate(initial_pn_top);

    //PrintState(cout, temp);

    //temp.UpdateNodes(initial_pn_base, initial_pn_top, initial_pn_base_mask, initial_pn_base_move, initial_pn_base_nodes, initial_pn_legals, goals);
    UpdateCrystal_base(temp, initial_pn_base_mask, initial_pn_top, initial_pn_base, initial_pn_legals, goals);

    is_terminal = initial_pn.data_init[terminal_crystal_id] & 0x4000;//initial_pn.GetTerminalNode()->holding_value;//IsTerminal(temp);
  }

  //PrintState(cout, temp);

  return GetGoals(temp);

   //for(size_t i = 0;i < role_size;i++)
    //cout << goals[i] << endl;

   //return goals;
}

inline void StateMachine::UpdateCrystal_move(const AMove& move, AMove& base, AState& top, std::set<size_t>* m_set, size_t* goals)
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t n_stack_index = 0;
  size_t v_stack_index = 0;

  for(size_t i = 0;i < move.get()->n_roles;i++)
  {
    if(move->moves[i] != base->moves[i])
    {
      //size_t start = util::Timer::microtimer();
      //n_stack.push(input_crystal_ids[i][base->moves[i]]);
      n_stack[n_stack_index++] = input_crystal_ids[i][base->moves[i]];
      //v_stack.push(0xffff);
      v_stack[v_stack_index++] = 0xffff;
      //n_stack.push(input_crystal_ids[i][move->moves[i]]);
      n_stack[n_stack_index++] = input_crystal_ids[i][move->moves[i]];
      //v_stack.push(0x0001);
      v_stack[v_stack_index++] = 0x0001;
      //stack_time += util::Timer::microtimer() - start;

      base->moves[i] = move->moves[i];
    }
  }

  while(n_stack_index)//!n_stack.empty())
  {
    size_t n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    propnet::CrystalNode& cn = initial_pn.cry[n_id];
    signed short& n_val = initial_pn.data_init[n_id];

    //size_t start = util::Timer::microtimer();
    //n_stack.pop();
    //v_stack.pop();
    //stack_time += util::Timer::microtimer() - start;

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(initial_pn.out_degree + cn.offset);

      n->CrystalUpdate(val, top, m_set, goals);
    }
    else
    {
      signed short t_val = val + n_val;

      if((t_val ^ n_val) & 0x4000)
      {
        signed short p_val = 0;
        if(t_val & 0x4000)
          p_val = 0x0001;
        else p_val = 0xffff;

        unsigned short* o_start = initial_pn.out_degree + cn.offset;

        for(size_t i = 0;i < cn.out_size;i++)
        {
          //size_t start = util::Timer::microtimer();
          n_stack[n_stack_index++] = o_start[i];
          v_stack[v_stack_index++] = p_val;
          //stack_time += util::Timer::microtimer() - start;
        }
      }
      n_val = t_val;
    }
  }
}

inline void StateMachine::UpdateCrystal_base(const AState& state, const AState& mask, AState& top, AState& base, std::set<size_t>* m_set, size_t* goals)
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t n_stack_index = 0;
  size_t v_stack_index = 0;

  for(size_t i = 0;i < state.get()->arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & mask.get()->s[i];
    if(x_or)
    {
      char s_val = state.get()->s[i];
      if(x_or & 1)
      {
        //n_stack.push(base_crystal_ids[8*i]);
        n_stack[n_stack_index++] = base_crystal_ids[8*i];
        if((bool)(s_val & 1))
        {
          v_stack[v_stack_index++] = 0x0001;
        }
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 2)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 1];
        if((bool)(s_val & 2))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 4)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 2];
        if((bool)(s_val & 4))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 8)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 3];
        if((bool)(s_val & 8))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 16)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 4];
        if((bool)(s_val & 16))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 32)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 5];
        if((bool)(s_val & 32))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 64)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 6];
        if((bool)(s_val & 64))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 128)
      {
        n_stack[n_stack_index++] = base_crystal_ids[8*i + 7];
        if((bool)(s_val & 128))
          v_stack[v_stack_index++] = 0x0001;
        else v_stack[v_stack_index++] = 0xffff;
      }
    }
  }

  while(n_stack_index)
  {
    size_t n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    //cout << n_id << " " << std::hex << val << std::dec << endl;

    //if(n_id == 164)
    //{
      //cout << "lol" << endl;
    //}

    propnet::CrystalNode& cn = initial_pn.cry[n_id];
    signed short& n_val = initial_pn.data_init[n_id];

    //n_stack.pop();
    //v_stack.pop();

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(initial_pn.out_degree + cn.offset);

      //cout << cn.offset << endl;

      //cout << std::hex << *(initial_pn.out_degree + cn.offset) << std::dec << endl;

      n->CrystalUpdate(val, top, m_set, goals);
    }
    else
    {
      signed short t_val = val + n_val;

      //cout << std::hex << n_val << endl;
      //cout << std::hex << t_val << std::dec << endl;

      if((t_val ^ n_val) & 0x4000)
      {
        signed short p_val = 0;
        if(t_val & 0x4000)
          p_val = 0x0001;
        else p_val = 0xffff;

        unsigned short* o_start = initial_pn.out_degree + cn.offset;

        //cout << (size_t)cn.out_size << " " << cn.offset << endl;

        for(size_t i = 0;i < cn.out_size;i++)
        {
          //cout << o_start[i] << endl;
          n_stack[n_stack_index++] = o_start[i];
          v_stack[v_stack_index++] = p_val;
        }
      }
      n_val = t_val;
    }
  }

  base.Equate(state);
}

}
