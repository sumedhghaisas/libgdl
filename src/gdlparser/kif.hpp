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

#include <gdlparser/parser/kif_driver.hpp>

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
 * This will generate a file 'test.dot' along 'test.kif' containing DOT representation
 * of dependency graph. This DOT file can be visualized with graph visualization libraries
 * like GraphViz.
 */
class KIF
{
    //! Some useful typedefs
    typedef parser::KIFDriver KIFDriver;

public:
    //! Constructor
    //!
    //! @param isWarn bool : enable or disable warnings
    //! @param stream std::ostream& : stream to print errors and warnings
    KIF(bool isWarn = true, std::ostream& stream = std::cout)
            : stream(&stream), isWarn(isWarn), driver(*this) {}

    ~KIF();

    //! Add given file as input
    //!
    //! @param filename const std::string& : filename of the file to be added
    //!
    void AddFile(const std::string& filename) { driver.AddFile(filename); }
    void AddFile(const std::vector<std::string>& files)
            { for(size_t i = 0;i < files.size();i++) AddFile(files[i]); }

    //! Parse the inputs
    //!
    //! @return bool : success or failure
    //!
    bool Parse() { return driver.Parse(); }

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

    //! get facts
    std::vector<Fact>& Facts() { return facts; }
    const std::vector<Fact>& Facts() const { return facts; }
    //! get clauses
    std::vector<Clause>& Clauses() { return clauses; }
    const std::vector<Clause> Clauses() const { return clauses; }

    const std::map<std::string, DGraphNode*>& DependencyGraph() { return dgraph; }

private:
    //! make KIFDriver class friend
    friend KIFDriver;

    //! add fact and clause to this kif -- used by KIFDriver
    void AddFact(const Fact& f) { facts.push_back(f); }
    void AddClause(const Clause& c) { clauses.push_back(c); }

    //! pointer to logging stream
    mutable std::ostream* stream;

    //! enable/disable warnings
    bool isWarn;

    //! All the facts
    std::vector<Fact> facts;

    //! All the clauses
    std::vector<Clause> clauses;

    //! dependency graph
    std::map<std::string, DGraphNode*> dgraph;

    //! driver to drive parsing
    KIFDriver driver;
};

} //namespace gdlparser

#endif // _GDLPARSER_KIF_HPP_INCLUDED
