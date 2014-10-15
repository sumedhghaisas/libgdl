/**
 * @file argument.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of Argument data type.
 */
#ifndef _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP
#define _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP

#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/unordered_map.hpp>

#include <libgdl/core/symbol_table/symbol_table.hpp>

#include "str_var_map.hpp"

namespace libgdl
{
namespace core /** Core functionality of libGDL **/
{

/**
 * Represents argument of fact or clause.
 * Can be relation, function or variable depending on type.
 * Stored as command(SymbolTable id) and arguments.
 *
 * @see SymbolTable, Fact, Clause
 */
struct Argument
{
  typedef std::vector<std::string> StringVec;

  //! Represents type of the argument (Relation, Function, Var)
  //!
  enum Type { Relation, Function, Var };

  //! Empty Constructor
  //!
  //!
  //!
  Argument()
    : t(Relation), val("") {}

  //! Copy constructor(deep copy)
  //! Does not throw any exception, can be used by stl wrappers
  //!
  //! \param arg Argument to copy
  //!
  //!
  Argument(const Argument& arg) noexcept;

  //! Constructs an argument from string, SymbolTable is used to encode
  //!
  //! \param str string representation
  //! \param symbol_table SymbolTable to encode
  //! \param isRel if argument is relation or not
  //! \param log Logging stream
  //! \return
  //!
  //!
  Argument(const std::string& str,
           SymbolTable symbol_table,
           bool isRel = true,
           Log log = GLOBAL_LOG);

  //! Move constructor
  //! Throws no exception, can be used by stl wrappers
  //!
  //! \param arg argument to move from
  //!
  //!
  Argument(Argument&& arg) noexcept
    : t(arg.t),
    val(std::move(arg.val)),
    value(std::move(arg.value)),
    args(std::move(arg.args))
  {}

  //! Destructor
  //!
  //!
  //!
  ~Argument();

  //! Swap function
  //!
  //! \param arg1 first argument
  //! \param arg2 second argument
  //! \return void
  //!
  //!
  friend inline void swap(Argument& arg1, Argument& arg2)
  {
    using std::swap;

    swap(arg1.t, arg2.t);
    swap(arg1.val, arg2.val);
    swap(arg1.value, arg2.value);
    swap(arg1.args, arg2.args);
  }

  //! copy-assignment operator
  //!
  //! \param arg argument to assign
  //! \return Argument&
  //!
  //!
  Argument& operator=(const Argument& arg);

  //! move assignment constructor
  //!
  //! \param arg argument to move assign
  //! \return Argument&
  //!
  //!
  Argument& operator=(Argument&& arg) { swap(*this, arg); return *this; }

  //! Destroys arguments to this argument before deletion
  //! called by destructor
  //! v_set stores set of variables already deleted
  //! hence they do not get deleted again
  //!
  //! \param v_set set of variables already deleted
  //! \return void
  //!
  //!
  void Destroy(std::set<Argument*>& v_set);

  //! Comparison operators
  bool operator==(const Argument& arg) const;
  //! Comparison operators
  bool operator!=(const Argument& arg) const { return !(*this == arg); }

  //! Special comparison operator
  //! Checks value and arguments(recursively check)
  //! for 'or' if given argument matches any argument then true is returned
  //!
  //! \param arg argument to check with
  //! \return bool
  //!
  //!
  bool OrEquate(const Argument& arg) const;

  //! Return true if given argument is there in arguments
  //!
  //! \param arg argument to check against
  //! \return bool
  //!
  //!
  bool HasAsArgument(const Argument& arg) const;

  //! Returns if current argument is ground (no variables)
  //!
  //! \return bool
  //!
  //!
  bool IsGround() const;

  //! Returns true if the argument is a negation
  //!
  //! \return bool
  //!
  //!
  bool IsNegation()
  {
    if(value == 0) return true;
    else return false;
  }

  //! Returns true if the argument is 'or'
  //!
  //! \return bool
  //!
  //!
  bool IsOr()
  {
    if(value == 1) return true;
    else return false;
  }

  //! Returns true if current argument is a variable
  //!
  //! \return bool
  //!
  //!
  bool IsVariable() const { if(t == Var) return true; else return false; }

  //! Returns true if this argument has any variables present
  //!dependy
  //! \return bool
  //!
  //!
  bool HasVariables() const;

  //! Returns variables present in this argument
  //!
  //! \return std::set<const Argument*>
  //!
  //!
  std::set<const Argument*> GetVariables() const;

  //! Get hash of this argument
  //!
  //! \return size_t
  //!
  //!
  size_t Hash();

  //! Returns string representation of this argument using the symbol table
  //! This function is used by SymbolDecodeStream to print argument
  //!
  //! \param symbol_table const SymbolTable&
  //! \return std::string
  //!
  //! @see SymbolDecodeStream
  //!
  std::string DecodeToString(const SymbolTable& symbol_table) const;

  //! type of this argument
  Type t;
  //! string representation if variable
  std::string val;
  //! symbol table id if function or relation
  size_t value;
  //! vector of arguments
  std::vector<Argument*> args;

  //! Constructs argument by copying a given argument and using the
  //! string to variable map given
  //!
  //! \param arg argument to copy
  //! \param v_map string to variable map
  //! \return Argument* new argument
  //!
  //!log
  static Argument* ConstructArgument(const Argument& arg,
                                     StrVarMap& v_map);

  //! Constructs argument from a string
  //!
  //! \param str string representation of the argument
  //! \param v_map string to variable map
  //! \param symbol_table SymbolTable to encode
  //! \param isRel if argument is relation or function
  //! \param log Logging stream
  //! \return
  //!
  //!
  static Argument* ConstructArgument(const std::string& str,
                                     StrVarMap& v_map,
                                     SymbolTable& symbol_table,
                                     bool isRel = true,
                                     Log log = GLOBAL_LOG);

  //! Separates a string input into command and arguments
  //!
  //! \param input string representation
  //! \param cmd command of the input given
  //! \param args arguments in the input
  //! \param log Logging stream
  //! \return
  //!
  //!
  static bool SeparateCommand(const std::string& input,
                              std::string& cmd,
                              std::vector<std::string>& args,
                              Log log = GLOBAL_LOG);

}; // struct Argument

}; // namespace core
}; //namespace libgdl


inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Argument& arg)
{
  if(arg.IsVariable())
  {
    o << arg.val;
    return o;
  }
  else if(arg.args.size() == 0)
  {
    o << arg.value;
    return o;
  }

  o << "( " << arg.value << " ";
  for(size_t i = 0;i < arg.args.size();i++) o << " " << *(arg.args[i]);
  o << " " << ")";
  return o;
}

inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Argument::Type& t)
{
  if(t == libgdl::core::Argument::Relation) o << "Relation";
  else if(t == libgdl::core::Argument::Function) o << "Function";
  else o << "Variable";
  return o;
}


#endif // _LIBGDL_CORE_DATATYPES_ARGUMENT_HPP
