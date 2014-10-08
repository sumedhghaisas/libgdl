/**
 * @file kif.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of KIF class.
 */
#ifndef __GDLPARSER_KIF_HPP_INCLUDED
#define __GDLPARSER_KIF_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include <libgdl/core.hpp>
#include <libgdl/core/data_types/error_type.hpp>
#include <libgdl/core/util/gdl_stream.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/dgraph/dgraph.hpp>

#include <libgdl/gdlparser/parser/kif_driver.hpp>
#include <libgdl/gdlparser/parser/kif_parser.tab.hh>

namespace libgdl
{
namespace gdlparser /** Parsing functionality of libGDL **/
{
/**
 * This class provides user interface to GDL parsing.
 *
 * example program demonstrates the usage of KIF class
 * @code
 * KIF kif;
 * kif.AddFile("arithmatic.kif");
 * kif.AddFile("8puzzle.kif");
 * kif.Parse();
 * @endcode
 *
 * To save the generated output to kif file
 *
 * @code
 * KIF kif;
 * kif.AddFile("3puzzle.kif");
 * kif.Parse();
 * kif.PrintToFile("test.kif");
 * kif.PrintDependencyGraph("test.dot");
 * @endcode
 *
 * This will generate a file 'test.dot' along 'test.kif' containing DOT
 * representation of dependency graph. This DOT file can be visualized with
 * graph visualization libraries like GraphViz.
 */
class KIF
{
  //! Some useful typedefs
  typedef parser::KIFDriver KIFDriver;
  typedef core::DGraph DGraph;
  typedef core::ErrorType ErrorType;
  typedef core::SymbolTable SymbolTable;
  typedef core::Argument Argument;
  typedef core::Fact Fact;
  typedef core::Clause Clause;
  typedef core::Location Location;

 public:
  //! Constructor
  //!
  //! \param isWarn Enable or disable warnings
  //! \param o_level Optimization level
  //! \param log Logging stream
  //!
  KIF(bool isWarn = true,
      char o_level = 1,
      const Log& log = std::cout);

  //! KIF destructor
  ~KIF();

  //! Add given file as input
  //!
  //! \param filename Filename of the file to be added
  //! \return returns True if file is successfully opened
  //!
  bool AddFile(const std::string& filename);

  //! Parse the inputs
  //! Returns true if parsing is successful
  //!
  //! \return bool
  //!
  bool Parse(bool ignoreErrors = false);

  //! Print the parsed knowledge to file
  //! Returns true if file is successfully written
  //!
  //! \param filename Output filename
  //! \return bool
  //!
  bool PrintToFile(const std::string& filename) const;

  //! Print dependency graph generated to file(DOT format).
  //! Returns true if file is successfully written
  //!
  //! \param filename Output filename
  //! \return bool
  //!
  //!
  bool PrintDependencyGraph(const std::string& filename) const;

  //! Clears all the files added and knowledge parsed
  void Clear()
  {
    streams.clear();
    facts.clear();
    clauses.clear();

    symbol_table = NULL;
    dgraph = NULL;
  }

  //! Access parsed facts(Read-Write)
  std::list<Fact>& Facts() { return facts; }
  //! Access parsed facts(Read)
  const std::list<Fact>& Facts() const { return facts; }

  //! Access parsed clauses(Read-Write)
  std::list<Clause>& Clauses() { return clauses; }
  //! Access parsed clauses(Read)
  const std::list<Clause> Clauses() const { return clauses; }

  //! Get generated dependency graph
  DGraph DependencyGraph()
  {
    return dgraph;
  }

  //! Get generated SymbolTable
  SymbolTable GetSymbolTable()
  {
    return symbol_table;
  }

  //! Get-Set logging stream
  Log& GetLog() { return log; }

 private:
  //! make KIFDriver class friend
  friend KIFDriver;

  //! Adds the given fact to parsed facts
  //! This function uses move semantics to add given fact to the list
  //! Returns reference to the added fact
  //!
  //! \param f Fact to be added
  //! \return const Fact&
  //!
  const Fact& AddFact(Fact&& f)
  {
    if(f.arg->value == SymbolTable::RoleID)
      isRole = true;
    else if(f.arg->value == SymbolTable::GoalID)
      isGoal = true;
    else if(f.arg->value == SymbolTable::TerminalID)
      isTerminal = true;
    else if(f.arg->value == SymbolTable::LegalID)
      isLegal = true;
    facts.push_front(std::move(f));
    return facts.front();
  }

  //! Adds the given clause to parsed clauses
  //! This function uses move semantics to add given clause to the list
  //! Returns reference to the added clause
  //!
  //! \param c Clause to be added
  //! \return const Clause&
  //!
  const Clause& AddClause(Clause&& c)
  {
    if(c.head->value == SymbolTable::GoalID)
      isGoal = true;
    else if(c.head->value == SymbolTable::TerminalID)
      isTerminal = true;
    else if(c.head->value == SymbolTable::LegalID)
      isLegal = true;
    clauses.push_front(std::move(c));
    return clauses.front();
  }

  //! Enable/disable warnings
  bool isWarn;

  //! Optimization level
  //! > 0: 'distinct' and 'not' premisses are reordered
  const char o_level;

  //! Parsed facts
  std::list<Fact> facts;

  //! Parsed clauses
  std::list<Clause> clauses;

  //! Streams added as input
  std::vector<util::GDLStream> streams;

  //! Parsing driver
  KIFDriver driver;

  //! Errors
  std::list<ErrorType> errors;
  //! Warnings
  std::list<ErrorType> warnings;

  //! Generated symbol table
  SymbolTable symbol_table;

  //! Generated dependency raph
  DGraph dgraph;

  //! Stores if role predicate is used in any input
  bool isRole;
  //! Stores if legal predicate is used in any input
  bool isLegal;
  //! Stores if terminal predicate is used in any input
  bool isTerminal;
  //! Stores if goal predicate is used in any input
  bool isGoal;

  //! Logging stream
  mutable Log log;

}; // class KIF

}; // namespace gdlparser
}; //namespace libgdl

#endif // _GDLPARSER_KIF_HPP_INCLUDED
