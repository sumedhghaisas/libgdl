#include "a_move.hpp"

#include <sstream>

#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>
#include <libgdl/core/data_types/argument.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

bool AMove::isPrintInitialized = false;
vector<vector<string>> AMove::str_input_props;

size_t RawAMove::n_roles = 0;

AMove AMove::Create(const list<string>& str_moves)
{
  if(!isPrintInitialized)
  {
    cerr << LOGID << "CreateMove function for AMove is not initialized. This function should be initialized by StateMachine." << endl;
    exit(1);
  }

  if(str_moves.size() != RawAMove::n_roles)
  {
    cerr << LOGID << "Incorrect size of list passed." << std::endl;
    cerr << LOGID << "Size: " << str_moves.size() << endl;
    cerr << LOGID << "Expected Size: " << RawAMove::n_roles << endl;
    return AMove("");
  }

  AMove out("");
  size_t r_index = 0;
  SymbolTable sym;
  for(auto it : str_moves)
  {
    size_t i_index = 0;
    Argument arg(it, sym, true);
    stringstream temp;
    SymbolDecodeStream temp2(sym, temp);
    temp2 << arg;
    string move_rep = temp.str();
    for(auto it2 : str_input_props[r_index])
    {
      if(it2 == move_rep)
        break;
      i_index++;
    }
    out->moves[r_index] = i_index;
    r_index++;
  }

  return out;
}

string AMove::GetStringRep(size_t role_id) const
{
  if(!isPrintInitialized)
  {
    GLOBAL_WARN << LOGID << "String representation of moves not initialized. \
            This representation is created and initialized by StateMachine."
           << endl;
    return "";
  }

  return str_input_props[role_id][get()->moves[role_id]] + " ";
}

void AMove::PrintMove(ostream& stream, const AMove& move)
{
  if(!isPrintInitialized)
  {
    stream << "Print function for AMove is not initialized. This function is created and initialized by StateMachine." << endl;
    return;
  }

  stream << "AMove:" << endl;

  for(size_t i = 0;i < RawAMove::n_roles;i++)
    stream << "\t" << str_input_props[i][move->moves[i]] << endl;
}
