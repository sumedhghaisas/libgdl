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
namespace gdlparser
{
/**
 * This class provides user interface to GDL parsing.
 *
 * example program demonstrates the usgae of KIF class
 * @code
 * KIF kif;
 * kif.AddFile("arithmatic.kif");
 * kif.AddFile("8puzzle.kif");
 * kif.Parse();
 * @endcode
 *
 * To save the generated output in kif file
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
  //! @param isWarn bool : enable or disable warnings
  //! @param stream std::ostream& : stream to print errors and warnings
  KIF(bool isWarn = true,
      char o_level = 0,
      const Log& log = std::cout);

  //! KIF destructor
  ~KIF();

  //! Add given file as input
  //!
  //! @param filename const std::string& : filename of the file to be added
  //! @return returns true if file is successfully opened
  //!
  bool AddFile(const std::string& filename);

  //! clears all the files added and knowledge
  void Clear()
  {
    streams.clear();
    facts.clear();
    clauses.clear();

    symbol_table = SymbolTable();
    dgraph = DGraph();
  }

  //! Parse the inputs
  //!
  //! @return bool : success or failure
  //!
  bool Parse(bool ignoreErrors = false);

  //! Print the parsed knowledge to file
  //!
  //! @param filename const std::string& : output filename
  //! @return bool : success or failure
  //!
  bool PrintToFile(const std::string& filename,
                   bool isDebuggingSymbols = false) const;

  //! Print dependency graph generated to file(DOT format).
  //!
  //! \param filename const std::string& : output filename
  //! \return bool : success or failure
  //!
  //!
  bool PrintDependencyGraph(const std::string& filename) const;

  //! Access facts
  std::list<Fact>& Facts() { return facts; }
  const std::list<Fact>& Facts() const { return facts; }

  //! Access Clauses
  std::list<Clause>& Clauses() { return clauses; }
  const std::list<Clause> Clauses() const { return clauses; }

  DGraph DependencyGraph()
  {
    return dgraph;
  }

  const SymbolTable& GetSymbolTable() const
  {
    return symbol_table;
  }
  SymbolTable& GetSymbolTable()
  {
    return symbol_table;
  }

  //! get this object's logging stream
  Log& GetLog() { return log; }

 private:
  //! make KIFDriver class friend
  friend KIFDriver;

  //void AddLineMark(const location_type& loc);

  //! add fact and clause to this kif -- used by KIFDriver
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

  void UpdateSymbolTable(const Argument& arg, const Location& loc);

  //! logging stream
  mutable Log log;

  //! enable/disable warnings
  bool isWarn;

  //! yet not implemented
  const char o_level;

  //! All the facts
  std::list<Fact> facts;

  //! All the clauses
  std::list<Clause> clauses;

  std::vector<util::GDLStream> streams;

  //! driver to drive parsing
  KIFDriver driver;

  std::list<ErrorType> errors;
  std::list<ErrorType> warnings;

  SymbolTable symbol_table;

  DGraph dgraph;

  bool isRole;
  bool isLegal;
  bool isTerminal;
  bool isGoal;
}; // class KIF

}; // namespace gdlparser
}; //namespace libgdl

#endif // _GDLPARSER_KIF_HPP_INCLUDED
