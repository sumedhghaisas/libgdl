namespace libgdl
{
namespace propnet
{

inline void PropNet::CrystalUpdate_input(const MoveType& move, PayLoadType& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t stack_index = 0;

  unsigned short*& n_stack = payload.n_stack;
  signed short*& v_stack = payload.v_stack;
  MoveType& base = payload.base_move;
  signed short*& data = payload.data;
  StateType& top = payload.top;
  size_t*& m_set_size = payload.legal_size;
  size_t*& goals = payload.goals;

  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
  {
    if(move->moves[i] != base->moves[i])
    {
      n_stack[stack_index] = input_crystal_ids[i][base->moves[i]];

      v_stack[stack_index] = 0xffff;

      stack_index++;

      n_stack[stack_index] = input_crystal_ids[i][move->moves[i]];

      v_stack[stack_index] = 0x0001;

      stack_index++;

      base->moves[i] = move->moves[i];
    }
  }

  while(stack_index)//!n_stack.empty())
  {
    stack_index--;

    unsigned short n_id = n_stack[stack_index];
    signed short val = v_stack[stack_index];

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    if(cn[0].type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
      n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
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

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          n_stack[stack_index] = cn[0].out_edges[i];
          v_stack[stack_index] = p_val;

          stack_index++;
        }
      }
      n_val = t_val;
    }
  }
}

inline bool PropNet::CrystalUpdate_base(const StateType& state, PayLoadType& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  size_t stack_index = 0;

  unsigned short*& n_stack = payload.n_stack;
  signed short*& v_stack = payload.v_stack;
  StateType& base = payload.base;
  signed short*& data = payload.data;
  StateType& top = payload.top;
  size_t*& m_set_size = payload.legal_size;
  size_t*& goals = payload.goals;

  for(size_t i = 0;i < StateType::RawType::arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & base_mask.get()->s[i];
    if(x_or)
    {
      char s_val = state.get()->s[i];
      if(x_or & 1)
      {
        //n_stack.push(base_crystal_ids[8*i]);
        n_stack[stack_index] = base_crystal_ids[8*i];
        if((bool)(s_val & 1))
        {
          v_stack[stack_index] = 0x0001;
        }
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 2)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 1];
        if((bool)(s_val & 2))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 4)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 2];
        if((bool)(s_val & 4))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 8)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 3];
        if((bool)(s_val & 8))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 16)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 4];
        if((bool)(s_val & 16))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 32)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 5];
        if((bool)(s_val & 32))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 64)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 6];
        if((bool)(s_val & 64))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 128)
      {
        n_stack[stack_index] = base_crystal_ids[8*i + 7];
        if((bool)(s_val & 128))
          v_stack[stack_index] = 0x0001;
        else v_stack[stack_index] = 0xffff;
        stack_index++;
      }
    }
  }

  while(stack_index)//!n_stack.empty())
  {
    stack_index--;

    unsigned short n_id = n_stack[stack_index];
    signed short val = v_stack[stack_index];

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    if(cn[0].type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
      n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
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

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          n_stack[stack_index] = cn[0].out_edges[i];
          v_stack[stack_index] = p_val;

          stack_index++;
        }
      }
      n_val = t_val;
    }
  }
  base.Equate(state);

  return payload.terminal;
}

inline void PropNet::GetRandomLegalMove(const PayLoadType& payload, MoveType& m) const
{
  for(size_t i = 0;i < role_size;i++)
  {
    size_t rnd = (rand() % payload.legal_size[i]) + 1;
    size_t index = 0;
    while(true)
    {
      if(payload.data[legal_memory_ids[i][index]] & 0x4000)
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
}

}
}
