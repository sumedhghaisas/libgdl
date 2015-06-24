#ifndef LIBGDL_PROPNET_PROPNET_PAYLOAD_HPP_INCLUDED
#define LIBGDL_PROPNET_PROPNET_PAYLOAD_HPP_INCLUDED

#include <libgdl/core/data_types/a_state.hpp>
#include <libgdl/core/data_types/a_move.hpp>

namespace libgdl
{
namespace propnet
{

struct PropNetPayLoad
{
  typedef AState StateType;
  typedef AMove MoveType;

  ~PropNetPayLoad();

  void Initialize(size_t crystal_data_size, size_t role_size, size_t stack_size);

  StateType top;
  StateType base;
  MoveType base_move;
  size_t* legal_size = NULL;
  size_t* goals = NULL;
  signed short* data = NULL;
  int* t_stack = NULL;
  bool terminal;

  char* crystal_buffer = NULL;
  size_t crystal_buffer_size = 0;
  bool isCrystallized = false;

  inline const StateType& GetState() const
  {
    return top;
  }

  inline size_t* GetGoals()
  {
    return goals;
  }

  void Crystallize(size_t data_init_size, size_t stack_size);
};

}
}


#endif // LIBGDL_PROPNET_PROPNET_PAYLOAD_HPP_INCLUDED
