#include "a_move.hpp"

#include <sstream>

#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>
#include <libgdl/core/data_types/argument.hpp>

using namespace std;
using namespace libgdl;
using namespace libgdl::core;

bool AMove::isCreateMoveInitialized = false;
AMove (*AMove::CreateMove)(const list<string>&) = NULL;

bool AMove::isPrintInitialized = false;
void (*AMove::Print)(ostream&, const AMove&) = NULL;

size_t RawAMove::n_roles = 0;

AMove AMove::ParseString(const std::string& str_moves)
{
  if(!isCreateMoveInitialized)
  {
    cerr << LOGID << "CreateMove function for AMove is not initialized. This function is created and initialized by StateMachine." << endl;
    exit(1);
  }

  SymbolTable sym;
  Argument arg(str_moves, sym, true);

  list<string> s_moves;
  for(auto it : arg.args)
  {
    stringstream stream;
    SymbolDecodeStream sds(sym, stream);
    sds << *it;
    cout << stream.str() << endl;
    s_moves.push_back(stream.str());
  }

  return CreateMove(s_moves);
}

void AMove::PrintMove(ostream& stream, const AMove& move)
{
  if(!isPrintInitialized)
  {
    stream << "Print function for AMove is not initialized. This function is created and initialized by StateMachine." << endl;
    return;
  }

  Print(stream, move);
}
