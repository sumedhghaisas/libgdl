/**
 * @file gdlreasoner.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of GDLReasoner.
 */
#include "gdlreasoner.hpp"

using namespace std;
using namespace libgdl::gdlreasoner;

GDLReasoner::GDLReasoner(const GDLReasoner& gdlr)
{
  const list<Argument*>& r_roles = gdlr.Roles();

  for(list<Argument*>::const_iterator it = r_roles.begin();
                                                      it != r_roles.end();it++)
  {
    roles.push_back(new Argument(**it));
  }

  init = new BState(new core::RawBState(*gdlr.InitState().get()));

  base_rules = gdlr.base_rules;
}

GDLReasoner::GDLReasoner(GDLReasoner&& gdlr)
{
  init = NULL;

  std::swap(init, gdlr.init);
  std::swap(roles, gdlr.roles);
  std::swap(base_rules, gdlr.base_rules);
}

GDLReasoner::~GDLReasoner()
{
  delete init;
  for(list<Argument*>::const_iterator it = roles.begin();it != roles.end();it++)
      delete *it;
}

GDLReasoner& GDLReasoner::operator=(const GDLReasoner& gdlr)
{
  delete init;
    for(std::list<Argument*>::const_iterator it = roles.begin();
                                                        it != roles.end();it++)
      delete *it;

  const list<Argument*>& r_roles = gdlr.Roles();

  for(list<Argument*>::const_iterator it = r_roles.begin();
                                                      it != r_roles.end();it++)
  {
    roles.push_back(new Argument(**it));
  }

  init = new BState(new core::RawBState(*gdlr.InitState().get()));
  base_rules = gdlr.base_rules;
  return *this;
}

GDLReasoner& GDLReasoner::operator=(GDLReasoner&& gdlr)
{
  swap(*this, gdlr);
  return *this;
}
