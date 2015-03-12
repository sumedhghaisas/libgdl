#include "a_state.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

bool AState::isPrintInitialized = false;
void (*AState::PrintAState)(ostream&, const AState&) = NULL;

void AState::PrintState(ostream& stream, const AState& state)
{
  if(!isPrintInitialized)
  {
    stream << "Print function not initialized for AState. This function is created and initialized by StateMachine." << endl;
    return;
  }

  PrintAState(stream, state);
}

size_t RawAState::arr_size = 0;
