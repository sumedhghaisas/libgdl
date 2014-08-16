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

#include <libgdl/gdlparser/parser/kif_driver.hpp>
#include <libgdl/gdlparser/parser/kif_parser.tab.hh>
#include <libgdl/core/util/gdl_stream.hpp>

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
  typedef parser::yy::KIFParser::location_type location_type;

  struct Symbol
  {
    Symbol(size_t id, const std::string& name)
      : id(id), name(name) {}

    size_t id;
    std::string name;
  };

 public:
  //! Constructor
  //!
  //! @param isWarn bool : enable or disable warnings
  //! @param stream std::ostream& : stream to print errors and warnings
  KIF(bool isWarn = true,
      bool isDebuggingSymbols = true,
      char o_level = 0,
      std::ostream* stream = &std::cout);

  //! KIF destructor
  ~KIF();

  //! Add given file as input
  //!
  //! @param filename const std::string& : filename of the file to be added
  //! @return returns true if file is successfully opened
  //!
  bool AddFile(const std::string& filename);

  //! clears all the files added and knowledge
  void Clear() { streams.clear(); facts.clear(); clauses.clear(); }

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
  bool PrintToFile(const std::string& filename) const;

  //! Print dependency graph generated to file(DOT format).
  //!
  //! \param filename const std::string& : output filename
  //! \return bool : success or failure
  //!
  //!
  bool PrintDependencyGraph(const std::string& filename) const;

  //! Access facts
  std::vector<Fact>& Facts() { return facts; }
  const std::vector<Fact>& Facts() const { return facts; }

  //! Access Clauses
  std::vector<Clause>& Clauses() { return clauses; }
  const std::vector<Clause> Clauses() const { return clauses; }

  //! get the dependency graph
  const std::map<std::string, DGraphNode*>& DependencyGraph() const
  {
    return dgraph;
  }

  //! set or reset debugging symbol generation
  bool DebuggingSymbolSupport() const { return isDebuggingSymbols; }

 private:
  //! make KIFDriver class friend
  friend KIFDriver;

  void AddLineMark(const location_type& loc);

  //! add fact and clause to this kif -- used by KIFDriver
  const Fact& AddFact(const Fact& f, const location_type& loc);
  const Fact& AddFact(Fact&& f, const location_type& loc);
  const Clause& AddClause(const Clause& c, const location_type& loc);
  const Clause& AddClause(Clause&& c, const location_type& loc);

  void UpdateSymbolTable(const Argument& arg, const Location& loc);

  //! pointer to logging stream
  mutable std::ostream* stream;

  //! enable/disable warnings
  bool isWarn;

  //! to generate debugging symbols in output file
  const bool isDebuggingSymbols;

  //! yet not implemented
  const char o_level;

  //! All the facts
  std::vector<Fact> facts;

  //! All the clauses
  std::vector<Clause> clauses;

  //! dependency graph
  std::map<std::string, DGraphNode*> dgraph;

  std::vector<util::GDLStream> streams;

  //! driver to drive parsing
  KIFDriver driver;

  //! index of the last fact tagged with "#line" location
  size_t f_last_index_with_linemark;
  //! index of the last clause tagged with "#line" location
  size_t c_last_index_with_linemark;
}; // class KIF

}; // namespace gdlparser
}; //namespace libgdl

#endif // _GDLPARSER_KIF_HPP_INCLUDED
