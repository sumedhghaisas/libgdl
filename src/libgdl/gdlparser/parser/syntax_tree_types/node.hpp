/**
 * @file node.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration Node class.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED

#include <stack>
#include <string>
#include <iostream>
#include <list>
#include <map>

#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/gdlparser/parser/location.hh>

namespace libgdl
{
namespace gdlparser
{
namespace parser
{
//! Forward declaration of KIFDriver class
class KIFDriver;

/**
 * This class represents parent class of all the nodes in generated(in parsing)
 * AST(Abstract Syntax Tree). The common functionality like location tracking
 * is implemented in this class. The pure virtual function CodeGen is used for
 * code generation from AST.
 *
 * @see KIFDriver, KIF
 */
class Node
{
 protected:
  //! Some useful imports
  typedef gdlparser::parser::yy::location location;
  typedef core::StrVarMap StrVarMap;

 public:
  //! Empty constructor
  //!
  //! \param loc Location of the node
  //!
  //!
  Node(const yy::location& loc)
    : loc(loc) {};

  //! Virtual destructor
  virtual ~Node() {}

  //! String operator for debugging purpose
  virtual operator std::string() const
  {
    std::cerr << "testing warning" << std::endl;
    return "testing";
  }

  //! Generate code
  //!
  //! \param driver Reference of the driver
  //! \param v_map String to GDL variable mapping
  //! \return void
  //!
  //!
  virtual void CodeGen(KIFDriver& driver,
                       StrVarMap& v_map) = 0;

  //! Get location of this node
  const location& GetLocation() { return loc; }

 protected:
   //! Location of the node
  location loc;
}; // class Node

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED
