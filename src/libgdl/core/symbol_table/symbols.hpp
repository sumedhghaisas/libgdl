/**
 * @file symbols.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Symbol.
 */
#ifndef _LIBGDL_CORE_SYMBOLS_HPP_INCLUDED
#define _LIBGDL_CORE_SYMBOLS_HPP_INCLUDED

#include <string>
#include <list>

#include <libgdl/core/data_types/location.hpp>

namespace libgdl
{

struct Symbol
{
  enum Type {FUNCTION, RELATION};

  Symbol(const Type& t, const std::string& name, const Location& loc)
    : t(t), name(name), loc(loc)
  {
    isUsed = false;
    isDef = false;
  }

  virtual ~Symbol() {}

  const Type& SymbolType() { return t; }
  const Location& GetLocation() { return loc; }

  const std::string& Name() { return name; }

  size_t Arity() { return arity; }

  void AddUsed(const Location& loc)
  {
    if(!isUsed)
    {
      isUsed = true;
      loc_used = loc;
    }
  }

  void AddDef(const Location& loc)
  {
    if(!isDef)
    {
      isDef = true;
      loc_def = loc;
    }
  }

  bool IsDef() { return isDef; }
  bool IsUsed() { return isUsed; }

  Type t;
  std::string name;
  Location loc;

  bool isUsed;
  Location loc_used;
  bool isDef;
  Location loc_def;

  size_t arity;
}; // class Symbol

struct FunctionSymbol : public Symbol
{
    FunctionSymbol(const std::string& name, size_t arity, const Location& loc)
        : Symbol(Symbol::FUNCTION, name, loc)
    {
        this->arity = arity;
    }
}; // class FunctionSymbol

struct RelationSymbol : public Symbol
{
    RelationSymbol(const std::string& name, size_t arity, const Location& loc)
        : Symbol(Symbol::RELATION, name, loc)
    {
        this->arity = arity;
    }
}; // RelationSymbol

}; // namespace libgdl

#endif // SYMBOLS_HPP_INCLUDED
