/**
 * @file basic_construct.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of BasicConstruct class.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_BASIC_CONSTRUCT_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_BASIC_CONSTRUCT_HPP_INCLUDED

#include <string>
#include <list>

#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/core/util/to_string.hpp>

#include <libgdl/core/data_types/location.hpp>

#include "node.hpp"

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

class TerminalHold
{};

template <class Policy, class Head, class Hold>
class BasicConstruct : public Node
{
  typedef libgdl::core::Argument Argument;
 public:
  BasicConstruct(Head* command, const yy::location& loc)
    : Node(loc), command(command)
  {}

  ~BasicConstruct()
  {
    delete command;
    for(typename std::list<Hold*>::iterator it = args.begin();it != args.end();it++)
      delete *it;
  }

  void AddArgument(Hold* gt)
  {
    args.push_back(gt);
  }
  void AddArgument(std::list<Hold*>& gts)
  {
    for(typename std::list<Hold*>::iterator it = gts.begin(); it != gts.end(); it++)
      args.push_back(*it);
  }

  operator std::string() const;

  void CodeGen(KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    Policy::CodeGen(driver, command, args, v_map, loc);
  }

  template<class T>
  bool CodeGen(T*& out,
               KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    return Policy::CodeGen(out, driver, command, args, v_map, loc);
  }

 private:
  Head* command;
  std::list<Hold*> args;
};

template <class Policy, class Head>
class BasicConstruct<Policy, Head, TerminalHold> : public Node
{
 public:
  BasicConstruct(Head* command, const yy::location& loc)
    : Node(loc), command(command) {}

  ~BasicConstruct()
  {
    delete command;
    for(typename std::list<BasicConstruct<Policy, Head, TerminalHold>*>::
                              iterator it = args.begin();it != args.end();it++)
      delete *it;
  }

  void AddArgument(BasicConstruct<Policy, Head, TerminalHold>* gt)
  {
    args.push_back(gt);
  }
  void AddArgument(std::list<BasicConstruct<Policy, Head, TerminalHold>*>& gts)
  {
    for(typename std::list<BasicConstruct<Policy, Head, TerminalHold>*>::
                              iterator it = gts.begin(); it != gts.end(); it++)
      args.push_back(*it);
  }

  operator std::string() const;

  void CodeGen(KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    Policy::CodeGen(driver, command, args, v_map, loc);
  }

  template<class T>
  bool CodeGen(T*& out,
               KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    return Policy::CodeGen(out, driver, command, args, v_map, loc);
  }

 private:
  Head* command;
  std::list<BasicConstruct<Policy, Head, TerminalHold>* > args;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl
#include "basic_construct_impl.hpp"

#endif // _LIBGDL_GDLPARSER_PARSER_BASIC_CONSTRUCT_HPP_INCLUDED
