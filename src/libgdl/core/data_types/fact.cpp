/**
 * @file fact.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of Fact data type.
 */
#include "fact.hpp"

using namespace libgdl;

Fact::Fact(const std::string& str) : isLocation(false)
{
  if(str.find("?") != std::string::npos)
  {
    std::cerr << "Unable to construct argument from " << str << std::endl;
    return;
  }

  arg = new Argument(str);
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
