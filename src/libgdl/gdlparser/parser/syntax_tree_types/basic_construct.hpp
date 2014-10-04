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

/**
 * This class acts as the dummy for Template specialization of BasicConstruct.
 *
 * @see BasicConstruct
 */
class TerminalHold
{};

/**
 * This class implements the basic construction of any valid syntax of GDL.
 * Every valid statement in GDL can be broken down in Head and Arguments.
 * For example, for a clause, consequent can be considered a Head and the tails
 * of the clause can be considered Arguments. This construction is templatized
 * by certain policy for sematical analysis. For example, for a clause,
 * BasicConstruct is templatized with ClausePolicy. For more information on this
 * check file syntax_tree_types_includes.hpp and syntax_tree_types_decl.hpp.
 *
 * \tparam Policy Policy for semantical analysis
 * \tparam Head The data structure of head
 * \tparam Hold The data structure of arguments
 *
 * @see Node
 */
template <class Policy, class Head, class Hold>
class BasicConstruct : public Node
{
  //! import Argument from libgdl::core
  typedef libgdl::core::Argument Argument;

 public:
  //! Empty constructor
  //!
  //! \param command Head of the construct
  //! \param loc Location of occurrence
  //!
  //!
  BasicConstruct(Head* command, const yy::location& loc)
    : Node(loc), command(command)
  {}

  //! Destructor
  ~BasicConstruct()
  {
    delete command;
    for(typename std::list<Hold*>::iterator it = args.begin();it != args.end();it++)
      delete *it;
  }

  //! Add argument to this construct
  //!
  //! \param gt Argument o add
  //! \return void
  //!
  //!
  void AddArgument(Hold* gt)
  {
    args.push_back(gt);
  }

  //! Add list of arguments to this construct
  //!
  //! \param gts List of arguments to add
  //! \return void
  //!
  //!
  void AddArgument(std::list<Hold*>& gts)
  {
    for(typename std::list<Hold*>::iterator it = gts.begin();it != gts.end();it++)
      args.push_back(*it);
  }

  //! String operator for debugging purpose
  operator std::string() const;

  //! Generate code for this construct
  //! The generated code is passed to KIFDriver object
  //! CodeGen of policy is responsible for this generation.
  //!
  //! \param driver KIFDriver object
  //! \param v_map String to variable mapping
  //! \return
  //!
  //!
  void CodeGen(KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    Policy::CodeGen(driver, command, args, v_map, loc);
  }

  //! Generates code and returns it as the first argument
  //! Returns success state
  //!
  //! \tparam T Class of returning object
  //! \param driver KIFDriver object
  //! \param v_map String to variable mapping
  //! \return bool
  //!
  //!
  template<class T>
  bool CodeGen(T*& out,
               KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    return Policy::CodeGen(out, driver, command, args, v_map, loc);
  }

 private:
  //! Head
  Head* command;
  //! List of arguments
  std::list<Hold*> args;
}; // class BasicConstruct

/**
 * Template specialized class of BasicConstruct. This specialization is used
 * when the GDL construction uses itself as Argument. For example, functions.
 * The argument to functions are itself function. This case cannot be
 * implemented without template specialization. The dummy class TerminalHold is
 * used for this specialization.
 *
 * \tparam Policy Policy for semantical analysis
 * \tparam Head The data structure of head
 *
 * @see BasicConstruct
 */
template <class Policy, class Head>
class BasicConstruct<Policy, Head, TerminalHold> : public Node
{
 public:
  //! Empty constructor
  //!
  //! \param command Head of the construct
  //! \param loc Location of occurrence
  //!
  //!
  BasicConstruct(Head* command, const yy::location& loc)
    : Node(loc), command(command) {}

  //! Destructor
  ~BasicConstruct()
  {
    delete command;
    for(typename std::list<BasicConstruct<Policy, Head, TerminalHold>*>::
                              iterator it = args.begin();it != args.end();it++)
      delete *it;
  }

  //! Add argument to this construct
  //!
  //! \param gt Argument o add
  //! \return void
  //!
  //!
  void AddArgument(BasicConstruct<Policy, Head, TerminalHold>* gt)
  {
    args.push_back(gt);
  }

  //! Add list of arguments to this construct
  //!
  //! \param gts List of arguments to add
  //! \return void
  //!
  //!
  void AddArgument(std::list<BasicConstruct<Policy, Head, TerminalHold>*>& gts)
  {
    for(typename std::list<BasicConstruct<Policy, Head, TerminalHold>*>::
                              iterator it = gts.begin(); it != gts.end(); it++)
      args.push_back(*it);
  }

  //! String operator for debugging purpose
  operator std::string() const;

  //! Generate code for this construct
  //! The generated code is passed to KIFDriver object
  //! CodeGen of policy is responsible for this generation.
  //!
  //! \param driver KIFDriver object
  //! \param v_map String to variable mapping
  //! \return
  //!
  //!
  void CodeGen(KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    Policy::CodeGen(driver, command, args, v_map, loc);
  }

  //! Generates code and returns it as the first argument
  //! Returns success state
  //!
  //! \tparam T Class of returning object
  //! \param driver KIFDriver object
  //! \param v_map String to variable mapping
  //! \return bool
  //!
  //!
  template<class T>
  bool CodeGen(T*& out,
               KIFDriver& driver,
               StrVarMap& v_map = StrVarMap())
  {
    return Policy::CodeGen(out, driver, command, args, v_map, loc);
  }

 private:
   //! Head
  Head* command;
  //! List of arguments
  std::list<BasicConstruct<Policy, Head, TerminalHold>* > args;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl
#include "basic_construct_impl.hpp"

#endif // _LIBGDL_GDLPARSER_PARSER_BASIC_CONSTRUCT_HPP_INCLUDED
