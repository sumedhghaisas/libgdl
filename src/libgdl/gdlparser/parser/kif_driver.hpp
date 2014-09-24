/**
 * @file kif_driver.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration driver class.
 */
#ifndef __GDLPARSER_KIF_DRIVER_HPP_INCLUDED
#define __GDLPARSER_KIF_DRIVER_HPP_INCLUDED

#include <string>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <stack>
#include <boost/unordered_map.hpp>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/error_type.hpp>
#include <libgdl/core/util/gdl_stream.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/dgraph/dgraph.hpp>

#include "kif_scanner.hpp"
#include "kif_parser.tab.hh"

namespace libgdl
{
namespace gdlparser
{
//! forward declaration of class KIF
class KIF;

namespace parser
{
/**
 * This class represents all the semantical analysis side of parsing.
 * Appropriate booking is done in this class to analyze the semantics of GDL
 * after parsing. This class implements symbol table to keep track of symbols
 * and their associated type and arity. Class generates dependency graph at the
 * end of the parsing to check for Unstratified rules( supports DOT format
 * string of dependency graph).
 * This driver supports multiple file parsing.
 */
class KIFDriver
{
  //! some useful typedefs
  typedef gdlparser::parser::yy::KIFParser::location_type location_type;
  typedef core::DGraph DGraph;
  typedef core::ErrorType ErrorType;
  typedef core::SymbolTable SymbolTable;
  typedef core::Fact Fact;
  typedef core::Clause Clause;
  typedef core::Argument Argument;

 public:
  //! constructs KIFDriver object
  KIFDriver(KIF &kif);

  //! destructor, deletes parser and scanner
  virtual ~KIFDriver();

  //! method to start the parsing
  bool Parse();

  const SymbolTable& GetSymbolTable() const;
  SymbolTable& GetSymbolTable();

  const DGraph& GetDGraph() const;
  DGraph& GetDGraph();

  void Error(const ErrorType& error);

  void Warn(const ErrorType& warn);

  const Fact& AddFact(Fact&& f_t);
  const Clause& AddClause(Clause&& c_t);

private:
  friend yy::KIFParser;
  friend KIFScanner;

  //! pointer to parser and scanner
  gdlparser::parser::yy::KIFParser *parser;
  gdlparser::parser::KIFScanner *scanner;

  //! reference to the calling object
  KIF& kif;

  std::vector<util::GDLStream>& streams;

  //! pointers which needs to be freed in destruction
  std::list<std::string*> to_free;

  size_t current_id;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _GDLPARSER_PARSER_KIF_DRIVER_HPP_INCLUDED
