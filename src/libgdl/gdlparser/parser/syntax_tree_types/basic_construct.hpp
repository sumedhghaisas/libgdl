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

#include <libgdl/core/data_types/variable_map.hpp>

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
  typedef libgdl::Argument Argument;

 public:
  BasicConstruct(Head* command, const yy::location& loc)
    : Node(loc), command(command) {}

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

  void CodeGen(Driver& driver,
               VariableMap& v_map = VariableMap())
  {
    Policy::CodeGen(driver, command, args, v_map);
  }

  template<class T>
  void CodeGen(T*& out,
               Driver& driver,
               VariableMap& v_map = VariableMap())
  {
    Policy::CodeGen(out, driver, command, args, v_map);
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

  void CodeGen(Driver& driver,
               VariableMap& v_map = VariableMap())
  {
    Policy::CodeGen(driver, command, args, v_map);
  }

  template<class T>
  void CodeGen(T*& out,
               Driver& driver,
               VariableMap& v_map = VariableMap())
  {
    Policy::CodeGen(out, driver, command, args, v_map);
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
