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
 * This class acts as the driver of the class KIFScanner and KIFParser and
 * performs semantical analysis on the parsed data.
 *
 * @see KIF, KIFScanner, KIFParser
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
  //! Constructs driver
  //! Driver stores the reference of the calling KIF object for information
  //! passing
  //!
  //! \param kif Calling object
  //!
  KIFDriver(KIF &kif);

  //! Destructor, deletes parser and scanner
  ~KIFDriver();

  //! Starts parsing
  bool Parse();

  //! Error tracking function
  //!
  //! \param error Raised error
  //!
  //!
  void Error(const ErrorType& error);

  //! Warning tracking function
  //!
  //! \param warn Raised warning
  //! \return void
  //!
  //!
  void Warn(const ErrorType& warn);

  //! Adds the given fact to parsed facts
  //! This function uses move semantics to add given fact to the list
  //! Returns reference to the added fact
  //!
  //! \param f Fact to be added
  //! \return const Fact&
  //!
  const Fact& AddFact(Fact&& f_t);

  //! Adds the given clause to parsed clauses
  //! This function uses move semantics to add given clause to the list
  //! Returns reference to the added clause
  //!
  //! \param c Clause to be added
  //! \return const Clause&
  //!
  const Clause& AddClause(Clause&& c_t);

  //! Access generated SymbolTable(Read-Write)
  const SymbolTable& GetSymbolTable() const;
  //! Access generated SymbolTable(Read)
  SymbolTable& GetSymbolTable();

  //! Access generated dependency graph(Read-Write)
  const DGraph& GetDGraph() const;
  //! Access generated dependency graph(Read)
  DGraph& GetDGraph();

private:
  friend yy::KIFParser;
  friend KIFScanner;

  //! Parser
  gdlparser::parser::yy::KIFParser *parser;

  //! Scanner
  gdlparser::parser::KIFScanner *scanner;

  //! Reference to the calling object
  KIF& kif;

  //! Input streams
  std::vector<util::GDLStream>& streams;

  //! Pointers which needs to be freed in destruction
  std::list<std::string*> to_free;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _GDLPARSER_PARSER_KIF_DRIVER_HPP_INCLUDED
