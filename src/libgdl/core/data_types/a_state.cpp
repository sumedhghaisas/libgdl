#include "a_state.hpp"

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

bool AState::isPrintInitialized = false;
vector<string> AState::str_base_props;

void AState::PrintState(ostream& stream, const AState& state)
{
  if(!isPrintInitialized)
  {
    stream << "Print function not initialized for AState." << endl;
    return;
  }

  stream << "AState: " << endl;
  size_t index = 0;
  bool temp = false;
  for(auto it : str_base_props)
  {
    state.Get(index, temp);
    if(temp)
      stream << "\t" << it << endl;
    index++;
  }
}

size_t RawAState::arr_size = 0;
