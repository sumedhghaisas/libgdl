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
void PropNet::GetPropNetBaseMask(StateType& s)
{
  s.Clear();
  for(auto it : base_nodes)
    if(del.find(it.second) == del.end())
      s.Set(it.first);
}

inline void PropNet::CrystalUpdate_input(const AMove& move, AMove& base, AState& top, Set<size_t>* m_set, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const
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
    unsigned short n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    propnet::CrystalNode& cn = cry[n_id];
    signed short& n_val = data[n_id];

    //size_t start = util::Timer::microtimer();
    //n_stack.pop();
    //v_stack.pop();
    //stack_time += util::Timer::microtimer() - start;

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(out_degree + cn.offset);

      //size_t start = util::Timer::microtimer();
      n->CrystalUpdate(val, top, m_set, goals);
      //n_count += util::Timer::microtimer() - start;
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

        unsigned short* o_start = out_degree + cn.offset;

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

inline void PropNet::CrystalUpdate_base(const AState& state, AState& base, AState& top, Set<size_t>* m_set, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t n_stack_index = 0;
  size_t v_stack_index = 0;

  for(size_t i = 0;i < state.get()->arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & base_mask.get()->s[i];
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
    unsigned short n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    //cout << n_id << " " << std::hex << val << std::dec << endl;

    //if(n_id == 164)
    //{
      //cout << "lol" << endl;
    //}

    //std::cout << n_id << std::endl;

    propnet::CrystalNode& cn = cry[n_id];
    signed short& n_val = data[n_id];

    //n_stack.pop();
    //v_stack.pop();

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(out_degree + cn.offset);

      //cout << cn.offset << endl;

      //cout << std::hex << *(initial_pn.out_degree + cn.offset) << std::dec << endl;
      //size_t start = util::Timer::microtimer();
      n->CrystalUpdate(val, top, m_set, goals);
      //n_count += util::Timer::microtimer() - start;
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

        unsigned short* o_start = out_degree + cn.offset;

        //cout << (size_t)cn.out_size << " " << cn.offset << endl;

        for(size_t i = 0;i < cn.out_size;i++)
        {
          //std::cout << "\t" << o_start[i] << std::endl;
          n_stack[n_stack_index++] = o_start[i];
          v_stack[v_stack_index++] = p_val;
        }
      }
      n_val = t_val;
    }
  }

  base.Equate(state);
}

inline void PropNet::UpdateNormal_base(const AState& state, AState& base, AState& top, Set<size_t>* m_set, size_t* goals) const
{
  AMove m;
  for(size_t i = 0;i < state.get()->arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & base_mask.get()->s[i];
    if(x_or)
    {
      char s_val = state.get()->s[i];
      if(x_or & 1)
      {
        //if(nodes[8*i] != NULL)
        arr_base_nodes[8*i]->Update((bool)(s_val & 1), base, top, m, m_set, goals);
      }

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
        //if(nodes[8*i + 1] != NULL)
        arr_base_nodes[8*i + 1]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
      //if(nodes[8*i + 2] != NULL)
        arr_base_nodes[8*i + 2]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
      //if(nodes[8*i + 3] != NULL)
        arr_base_nodes[8*i + 3]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
        //if(nodes[8*i + 4] != NULL)
        arr_base_nodes[8*i + 4]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
        //if(nodes[8*i + 5] != NULL)
        arr_base_nodes[8*i + 5]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
        //if(nodes[8*i + 6] != NULL)
        arr_base_nodes[8*i + 6]->Update((bool)(s_val & 1), base, top, m, m_set, goals);

      x_or = x_or >> 1;
      s_val = s_val >> 1;
      if(x_or & 1)
        //if(nodes[8*i + 7] != NULL)
        arr_base_nodes[8*i + 7]->Update((bool)(s_val & 1), base, top, m, m_set, goals);
    }
  }
}

inline void PropNet::UpdateNormal_input(const AMove& move, AState& base, AState& top, AMove& m, Set<size_t>* m_set, size_t* goals) const
{
  for(size_t i = 0;i < move.get()->n_roles;i++)
  {
    if(move.get()->moves[i] != m->moves[i])
    {
      arr_input_nodes[i][m->moves[i]]->Update(false, base, top, m, m_set, goals);
      arr_input_nodes[i][move.get()->moves[i]]->Update(true, base, top, m, m_set, goals);
    }
  }
}

inline void PropNet::CrystalUpdate_input2(const AMove& move, AMove& base, AState& top, bool** m_set, size_t* m_set_size, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const
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
    unsigned short n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    propnet::CrystalNode& cn = cry[n_id];
    signed short& n_val = data[n_id];

    //size_t start = util::Timer::microtimer();
    //n_stack.pop();
    //v_stack.pop();
    //stack_time += util::Timer::microtimer() - start;

    if(cn.type)
    {
      propnet::node_types::NextNode* n = (propnet::node_types::NextNode*)*(size_t*)(out_degree + cn.offset);

      //size_t start = util::Timer::microtimer();
      n->CrystalUpdate(val, top, m_set, m_set_size, goals);
      //n_count += util::Timer::microtimer() - start;
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

        unsigned short* o_start = out_degree + cn.offset;

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

inline void PropNet::CrystalUpdate_base2(const AState& state, AState& base, AState& top, bool** m_set, size_t* m_set_size, size_t* goals, signed short* data, unsigned short* n_stack, signed short* v_stack) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t n_stack_index = 0;
  size_t v_stack_index = 0;

  for(size_t i = 0;i < state.get()->arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & base_mask.get()->s[i];
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

  while(n_stack_index)//!n_stack.empty())
  {
    unsigned short n_id = n_stack[--n_stack_index];
    signed short val = v_stack[--v_stack_index];

    propnet::CrystalNode& cn = cry[n_id];
    signed short& n_val = data[n_id];

    //size_t start = util::Timer::microtimer();
    //n_stack.pop();
    //v_stack.pop();
    //stack_time += util::Timer::microtimer() - start;

    if(cn.type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)(out_degree + cn.offset);

      //size_t start = util::Timer::microtimer();
      n->CrystalUpdate(val, top, m_set, m_set_size, goals);
      //n_count += util::Timer::microtimer() - start;
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

        unsigned short* o_start = out_degree + cn.offset;

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
  base.Equate(state);
}


}
}
