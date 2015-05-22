namespace libgdl
{
namespace propnet
{

inline void PropNet::Update(const MoveType& move, PayLoadType& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  using namespace crystallization;

  int* pay_stack = payload.t_stack;
  int* t_stack = pay_stack;
  MoveType& base = payload.base_move;
  signed short* data = payload.data;
  StateType& top = payload.top;
  size_t* m_set_size = payload.legal_size;
  size_t* goals = payload.goals;

  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
  {
    if(move->moves[i] != base->moves[i])
    {
      //n_stack[stack_index] = input_crystal_ids[i][base->moves[i]];
      *((unsigned short*)t_stack) = input_crystal_ids[i][base->moves[i]];

      //v_stack[stack_index] = CrystalConfig::CrystalDecrementVal;
      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;

      t_stack++;

      *((unsigned short*)t_stack) = input_crystal_ids[i][move->moves[i]];

      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;

      t_stack++;

      base->moves[i] = move->moves[i];
    }
  }


  while(t_stack != pay_stack)//!n_stack.empty())
  {
    t_stack--;

    unsigned short n_id = *((unsigned short*)t_stack);
    signed short val = *(((signed short*)t_stack) + 1);

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    signed short t_val = val + n_val;
    if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
    {
      if(cn[0].type)
      {
        propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
        n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
      }
      else
      {
        signed short p_val = 0;
        if(t_val & CrystalConfig::CrystalSignMask)
          p_val = CrystalConfig::CrystalIncrementVal;
        else p_val = CrystalConfig::CrystalDecrementVal;

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          *((unsigned short*)t_stack) = cn[0].out_edges[i];
          *(((signed short*)t_stack) + 1) = p_val;

          t_stack++;
        }
      }
    }

    n_val = t_val;
//    if(cn[0].type)
//    {
//      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
//      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
//      n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
//    }
//    else
//    {
//      signed short t_val = val + n_val;
//
//      if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
//      {
//        signed short p_val = 0;
//        if(t_val & CrystalConfig::CrystalSignMask)
//          p_val = CrystalConfig::CrystalIncrementVal;
//        else p_val = CrystalConfig::CrystalDecrementVal;
//
//        for(size_t i = 0;i < cn[0].out_size;i++)
//        {
//          *((unsigned short*)t_stack) = cn[0].out_edges[i];
//          *(((signed short*)t_stack) + 1) = p_val;
//
//          t_stack++;
//        }
//      }
//      n_val = t_val;
//    }
  }
}

inline void PropNet::Update2(const MoveType& move, PayLoadType2& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  using namespace crystallization;

  int* pay_stack = payload.t_stack;
  int* t_stack = pay_stack;
  MoveType& base = payload.base_move;
  signed short* data = payload.data;
  StateType& top = payload.top;
  MoveSet*& m_set = payload.m_set;
  size_t* goals = payload.goals;

  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
  {
    if(move->moves[i] != base->moves[i])
    {
      //n_stack[stack_index] = input_crystal_ids[i][base->moves[i]];
      *((unsigned short*)t_stack) = input_crystal_ids[i][base->moves[i]];

      //v_stack[stack_index] = CrystalConfig::CrystalDecrementVal;
      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;

      t_stack++;

      *((unsigned short*)t_stack) = input_crystal_ids[i][move->moves[i]];
      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;

      t_stack++;

      base->moves[i] = move->moves[i];
    }
  }

  while(t_stack != pay_stack)//!n_stack.empty())
  {
    t_stack--;

    unsigned short n_id = *((unsigned short*)t_stack);
    signed short val = *(((signed short*)t_stack) + 1);

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    if(cn[0].type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
      n->CrystalUpdate(val, top, n_val, m_set, goals, payload.terminal);
    }
    else
    {
      signed short t_val = val + n_val;

      if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
      {
        signed short p_val = 0;
        if(t_val & CrystalConfig::CrystalSignMask)
          p_val = CrystalConfig::CrystalIncrementVal;
        else p_val = CrystalConfig::CrystalDecrementVal;

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          *((unsigned short*)t_stack) = cn[0].out_edges[i];
          *(((signed short*)t_stack) + 1) = p_val;

          t_stack++;
        }
      }
      n_val = t_val;
    }
  }
}

inline void PropNet::Update3(MoveSet::const_iterator* move, PayLoadType2& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  using namespace crystallization;

  int* pay_stack = payload.t_stack;
  int* t_stack = pay_stack;
  MoveType& base = payload.base_move;
  signed short* data = payload.data;
  StateType& top = payload.top;
  MoveSet*& m_set = payload.m_set;
  size_t* goals = payload.goals;

  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
  {
    if(*move[i] != base->moves[i])
    {
      //n_stack[stack_index] = input_crystal_ids[i][base->moves[i]];
      *((unsigned short*)t_stack) = input_crystal_ids[i][base->moves[i]];

      //v_stack[stack_index] = CrystalConfig::CrystalDecrementVal;
      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;

      t_stack++;

      *((unsigned short*)t_stack) = input_crystal_ids[i][*move[i]];
      *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;

      t_stack++;

      base->moves[i] = *move[i];
    }
  }

  while(t_stack != pay_stack)//!n_stack.empty())
  {
    t_stack--;

    unsigned short n_id = *((unsigned short*)t_stack);
    signed short val = *(((signed short*)t_stack) + 1);

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    if(cn[0].type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
      n->CrystalUpdate(val, top, n_val, m_set, goals, payload.terminal);
    }
    else
    {
      signed short t_val = val + n_val;

      if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
      {
        signed short p_val = 0;
        if(t_val & CrystalConfig::CrystalSignMask)
          p_val = CrystalConfig::CrystalIncrementVal;
        else p_val = CrystalConfig::CrystalDecrementVal;

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          *((unsigned short*)t_stack) = cn[0].out_edges[i];
          *(((signed short*)t_stack) + 1) = p_val;

          t_stack++;
        }
      }
      n_val = t_val;
    }
  }
}

inline bool PropNet::Update(const StateType& state, PayLoadType& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  //unsigned short*& n_stack = payload.n_stack;
  //signed short*& v_stack = payload.v_stack;
  using namespace crystallization;

  int* pay_stack = payload.t_stack;
  int* t_stack = pay_stack;
  StateType& base = payload.base;
  signed short*& data = payload.data;
  StateType& top = payload.top;
  size_t*& m_set_size = payload.legal_size;
  size_t*& goals = payload.goals;

  size_t start = util::Timer::microtimer();
  for(size_t i = 0;i < StateType::RawType::arr_size;i++)
  {
    char x_or = state.get()->s[i] ^ base.get()->s[i];
    x_or = x_or & base_mask.get()->s[i];
    if(x_or)
    {
      char s_val = state.get()->s[i];
      //char t_val = s_val & x_or;
      if(x_or & 1)
      {
        //n_stack.push(base_crystal_ids[8*i]);
        *((unsigned short*)t_stack) = base_crystal_ids[8*i];
        if(s_val & 1)
        {
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        }
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 2)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 1];
        if(s_val & 2)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 4)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 2];
        if(s_val & 4)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 8)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 3];
        if(s_val & 8)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 16)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 4];
        if(s_val & 16)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 32)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 5];
        if(s_val & 32)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 64)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 6];
        if(s_val & 64)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 128)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 7];
        if(s_val & 128)
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }
    }
  }
  debug_time += util::Timer::microtimer() - start;

  while(t_stack != pay_stack)//!n_stack.empty())
  {
    t_stack--;

    unsigned short n_id = *((unsigned short*)t_stack);
    signed short val = *(((signed short*)t_stack) + 1);

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    signed short t_val = val + n_val;
    if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
    {
      if(cn[0].type)
      {
        propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
        n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
      }
      else
      {
        signed short p_val = 0;
        if(t_val & CrystalConfig::CrystalSignMask)
          p_val = CrystalConfig::CrystalIncrementVal;
        else p_val = CrystalConfig::CrystalDecrementVal;

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          *((unsigned short*)t_stack) = cn[0].out_edges[i];
          *(((signed short*)t_stack) + 1) = p_val;

          t_stack++;
        }
      }
    }

    n_val = t_val;

//    if(cn[0].type)
//    {
//      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
//      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
//      n->CrystalUpdate(val, top, n_val, m_set_size, goals, payload.terminal);
//    }
//    else
//    {
//      signed short t_val = val + n_val;
//
//      if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
//      {
//        signed short p_val = 0;
//        if(t_val & CrystalConfig::CrystalSignMask)
//          p_val = CrystalConfig::CrystalIncrementVal;
//        else p_val = CrystalConfig::CrystalDecrementVal;
//
//        for(size_t i = 0;i < cn[0].out_size;i++)
//        {
//          *((unsigned short*)t_stack) = cn[0].out_edges[i];
//          *(((signed short*)t_stack) + 1) = p_val;
//
//          t_stack++;
//        }
//      }
//      n_val = t_val;
//    }
  }
  base.Equate(state);

  return payload.terminal;
}

inline bool PropNet::Update2(const StateType& state, PayLoadType2& payload) const
{
  //std::stack<size_t, std::deque<size_t>> n_stack;
  //std::stack<signed short, std::deque<size_t>> v_stack;

  //unsigned short*& n_stack = payload.n_stack;
  //signed short*& v_stack = payload.v_stack;
  using namespace crystallization;

  int* pay_stack = payload.t_stack;
  int* t_stack = pay_stack;
  StateType& base = payload.base;
  signed short*& data = payload.data;
  StateType& top = payload.top;
  MoveSet*& m_set = payload.m_set;
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
        *((unsigned short*)t_stack) = base_crystal_ids[8*i];
        if((bool)(s_val & 1))
        {
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        }
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 2)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 1];
        if((bool)(s_val & 2))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 4)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 2];
        if((bool)(s_val & 4))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 8)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 3];
        if((bool)(s_val & 8))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 16)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 4];
        if((bool)(s_val & 16))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 32)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 5];
        if((bool)(s_val & 32))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 64)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 6];
        if((bool)(s_val & 64))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }

      //x_or = x_or >> 1;
      //s_val = s_val >> 1;
      if(x_or & 128)
      {
        *((unsigned short*)t_stack) = base_crystal_ids[8*i + 7];
        if((bool)(s_val & 128))
          *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalIncrementVal;
        else *(((signed short*)t_stack) + 1) = CrystalConfig::CrystalDecrementVal;
        t_stack++;
      }
    }
  }

  while(t_stack != pay_stack)//!n_stack.empty())
  {
    t_stack--;

    unsigned short n_id = *((unsigned short*)t_stack);
    signed short val = *(((signed short*)t_stack) + 1);

    propnet::CrystalNode* cn = (propnet::CrystalNode*)(arr_propnet + n_id);
    signed short& n_val = data[cn[0].data_id];

    if(cn[0].type)
    {
      propnet::node_types::Node* n = (propnet::node_types::Node*)*(size_t*)cn[0].out_edges;
      //std::cout << n_id << " " << std::hex << n << std::dec << std::endl;
      n->CrystalUpdate(val, top, n_val, m_set, goals, payload.terminal);
    }
    else
    {
      signed short t_val = val + n_val;

      if((t_val ^ n_val) & CrystalConfig::CrystalSignMask)
      {
        signed short p_val = 0;
        if(t_val & CrystalConfig::CrystalSignMask)
          p_val = CrystalConfig::CrystalIncrementVal;
        else p_val = CrystalConfig::CrystalDecrementVal;

        for(size_t i = 0;i < cn[0].out_size;i++)
        {
          *((unsigned short*)t_stack) = cn[0].out_edges[i];
          *(((signed short*)t_stack) + 1) = p_val;

          t_stack++;
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
  using namespace crystallization;

  for(size_t i = 0;i < role_size;i++)
  {
    size_t rnd = (rand() % payload.legal_size[i]) + 1;
    size_t index = 0;
    while(true)
    {
      if(payload.data[legal_memory_ids[i][index]] & CrystalConfig::CrystalSignMask)
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
