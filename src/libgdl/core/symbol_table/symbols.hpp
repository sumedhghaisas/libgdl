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

#include <libgdl/gdlparser/parser/location.hh>

namespace libgdl
{

struct Symbol
{
  typedef gdlparser::parser::yy::location location;

  enum Type {FUNCTION, RELATION};

  Symbol(const Type& t, const std::string& name, const location& loc)
    : t(t), name(name), loc(loc)
  {
    isUsed = false;
    isDef = false;
  }

  virtual ~Symbol() {}

  const Type& SymbolType() { return t; }
  const location& Location() { return loc; }

  const std::string& Name() { return name; }

  size_t Arity() { return arity; }

  void AddUsed(const location& loc)
  {
    if(!isUsed)
    {
      isUsed = true;
      loc_used = loc;
    }
  }

  void AddDef(const location& loc)
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
  location loc;
  bool isUsed;
  location loc_used;
  bool isDef;
  location loc_def;

  size_t arity;
}; // class Symbol

struct FunctionSymbol : public Symbol
{
    FunctionSymbol(const std::string& name, size_t arity, const location& loc)
        : Symbol(Symbol::FUNCTION, name, loc)
    {
        this->arity = arity;
    }
}; // class FunctionSymbol

struct RelationSymbol : public Symbol
{
    RelationSymbol(const std::string& name, size_t arity, const location& loc)
        : Symbol(Symbol::RELATION, name, loc)
    {
        this->arity = arity;
    }
}; // RelationSymbol

}; // namespace libgdl

#endif // SYMBOLS_HPP_INCLUDED
