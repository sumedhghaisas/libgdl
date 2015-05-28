#include "propnet_payload.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::propnet;

PropNetPayLoad::~PropNetPayLoad()
{
  if(isCrystallized)
  {
    base->s = NULL;
    top->s = NULL;
    base_move->moves = NULL;
    delete[] crystal_buffer;
  }
  else
  {
    delete[] legal_size;
    delete[] goals;
    delete[] data;
    delete[] t_stack;
  }
}

void PropNetPayLoad::Crystallize(size_t data_init_size, size_t stack_size)
{
  size_t mem_size_in_bytes = 0;

  size_t state_size = StateType::RawType::arr_size * sizeof(char);
  size_t move_size = MoveType::RawType::n_roles * sizeof(MoveType::RawType::BuffType);
  size_t legal_size_size = MoveType::RawType::n_roles * sizeof(size_t);
  size_t goal_size = MoveType::RawType::n_roles * sizeof(size_t);
  size_t data_size = data_init_size * sizeof(signed short);
  size_t t_stack_size = stack_size * sizeof(int);

  //! Add the memory required for base
  mem_size_in_bytes += state_size;
  //! Add the memory required for top
  mem_size_in_bytes += state_size;
  //! Add the memory required for base_move
  mem_size_in_bytes += move_size;
  //! Add the memory required for legal_size
  mem_size_in_bytes += legal_size_size;
  //! Add the memory required for goals
  mem_size_in_bytes += goal_size;
  //! Add the memory required for data
  mem_size_in_bytes += data_size;
  //! Add the memory required for n_stack
  mem_size_in_bytes += t_stack_size;

  crystal_buffer = new char[mem_size_in_bytes];

  char* current = crystal_buffer;

  //! Relocate base to crystal
  base.Relocate(current);
  current = current + state_size;

  //! Relocate top to crystal
  top.Relocate(current);
  current = current + state_size;

  //! Relocate base_move to crystal
  base_move.Relocate((size_t*)current);
  current = current + move_size;

  //! Relocate data to crystal
  signed short* data_t = (signed short*)current;
  for(size_t i = 0;i < data_init_size;i++)
    data_t[i] = data[i];
  delete[] data;
  data = data_t;
  current = current + data_size;

  //! Relocate t_stack to crystal
  int* t_stack_t = (int*)current;
  for(size_t i = 0;i < stack_size;i++)
    t_stack_t[i] = t_stack[i];
  delete[] t_stack;
  t_stack = t_stack_t;
  current = current + t_stack_size;

  //! Relocate legal_size to crystal
  size_t* legal_size_t = (size_t*)current;
  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
    legal_size_t[i] = legal_size[i];
  delete[] legal_size;
  legal_size = legal_size_t;
  current = current + legal_size_size;

  //! Relocate goals to crystal
  size_t* goals_t = (size_t*)current;
  for(size_t i = 0;i < MoveType::RawType::n_roles;i++)
    goals_t[i] = goals[i];
  delete[] goals;
  goals = goals_t;
  current = current + goal_size;

  isCrystallized = true;
}

void PropNetPayLoad::Initialize(size_t crystal_data_size, size_t role_size, size_t stack_size)
{
  data = new signed short[crystal_data_size];
  top = StateType();
  goals = new size_t[role_size];
  base = StateType();
  legal_size = new size_t[role_size];

  t_stack = new int[stack_size];
  base_move = MoveType();

  terminal = false;

  for(size_t i = 0;i < role_size;i++)
  {
    goals[i] = 0;
    legal_size[i] = 0;
  }
}
