/**
 * @file fact.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Fact data type.
 */
#include "fact.hpp"

using namespace libgdl;

Fact::Fact(const std::string& str,
           SymbolTable& symbol_table,
           Log log)
  : isLocation(false)
{
  if(str.find("?") != std::string::npos)
  {
    log.Fatal << "Unable to construct argument from " << str << std::endl;
    return;
  }

  arg = new Argument(str, symbol_table, true, log);
}

Fact& Fact::operator=(const Fact& f)
{
  std::set<Argument*> v_set;
  arg->Destroy(v_set);

  arg = new Argument(*f.arg);
  isLocation = f.isLocation;
  loc = f.loc;
  return *this;
}
