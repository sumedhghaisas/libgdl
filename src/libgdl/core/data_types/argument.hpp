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
namespace core
{

/**
 * Represents argument of fact or clause.
 * Can be relation, function or variable depending on type.
 * Stored as command and arguments.
 *
 * @see KIFDriver
 */
struct Argument
{
  typedef std::vector<std::string> StringVec;

  //! enum type
  enum Type { Relation, Function, Var };

  //! empty constructor
  Argument() : t(Relation), val("") {}
  //! copy constructor
  Argument(const Argument& arg) noexcept;
  //! construct argument from string
  Argument(const std::string& str,
           SymbolTable& symbol_table,
           bool isRel = true,
           Log log = std::cerr);
  //! move constructor
  Argument(Argument&& arg) noexcept
    : t(arg.t), val(std::move(arg.val)), value(std::move(arg.value)),
    args(std::move(arg.args)) {}

  //! Destructor
  ~Argument();

  //! swap function
  friend inline void swap(Argument& arg1, Argument& arg2)
  {
    using std::swap;

    swap(arg1.t, arg2.t);
    swap(arg1.val, arg2.val);
    swap(arg1.value, arg2.value);
    swap(arg1.args, arg2.args);
  }

  //! copy-assignment operator
  Argument& operator=(const Argument& arg);
  //! move assignment constructor
  Argument& operator=(Argument&& arg) { swap(*this, arg); return *this; }

  //! Destroys arguments to this argument before deletion
  //! called by destructor
  //! v_set is the set of variables already deleted
  //! updates it accordingly
  void Destroy(std::set<Argument*>& v_set);

  //! comparison operator
  bool operator==(const Argument& arg) const;
  bool operator!=(const Argument& arg) const { return !(*this == arg); }

  //! special comparison operator
  //! checks value and arguments(recursively check)
  //! for 'or' if given argument matches any argument to 'or' true is returned
  bool OrEquate(const Argument& arg) const;

  //! return true if given argument is there in arguments
  bool HasAsArgument(const Argument& arg) const;

  //! returns if current argument is ground
  bool IsGround() const;

  //! returns true if the argument is a negation
  bool IsNegation()
  {
    if(value == 0) return true;
    else return false;
  }

  //! returns true if the argument is 'or'
  bool IsOr()
  {
    if(value == 1) return true;
    else return false;
  }

  //! returns if current argument is a variable
  bool IsVariable() const { if(t == Var) return true; else return false; }

  bool HasVariables() const;

  std::set<const Argument*> GetVariables() const;

  //! compute hash value
  size_t Hash(const boost::unordered_map<std::string, size_t>& id_map);

  std::string DecodeToString(const SymbolTable& symbol_table) const;

  //! type of this argument
  Type t;
  //! command value
  std::string val;
  size_t value;
  //! vector of arguments
  std::vector<Argument*> args;

  //! used by copy constructors
  static Argument* ConstructArgument(const Argument& arg,
                                     StrVarMap& v_map);
  static Argument* ConstructArgument(const std::string& str,
                                     StrVarMap& v_map,
                                     SymbolTable& symbol_table,
                                     bool isRel = true,
                                     Log log = std::cerr);

  //! separates a string input into command and arguments
  static bool SeparateCommand (const std::string& input,
                               std::string& cmd,
                               std::vector<std::string>& args,
                               Log log = std::cerr);

}; // struct Argument

}; // namespace core
}; //namespace libgdl


inline std::ostream& operator<<(std::ostream& o,
                                const libgdl::core::Argument& arg)
{
  if(arg.args.size() == 0)
  {
    o << arg.val;
    return o;
  }
  else o << "( " + arg.val;

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
