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
 * KIF kif("test.kif");
 * kif.AddFile("arithmatic.kif");
 * kif.AddFile("8puzzle.kif");
 * kif.Parse();
 * @endcode
 *
 * This code will generate output file named 'test.kif' containing compiled facts
 * and clauses. By default all the errors and warning will be displayed on std::cout.
 * This can be changed by passing a different stream as the third parameter to KIF
 * constructor. KIF can also save the dependency graph created in DOT format.
 *
 * @code
 * KIF kif("test.kif", true, stream);
 * kif.AddFile("3puzzle.kif");
 * kif.GraphFilename() = "dgraph.dot";
 * kif.Parse();
 * @endcode
 *
 * This will generate a file 'dgraph.dot' along 'test.kif' containing DOT representation
 * of dependency graph. This DOT file can be visualized with graph visualization libraries
 * like GraphViz.
 *
 * @note if output_filename is left blank no output will be generated.
 */
class KIF
{
    //! Some useful typedefs
    typedef parser::KIFDriver KIFDriver;

public:
    //! Constructor
    //!
    //! @param output_filename const std::string& : filename for generating output
    //! @param saveGraph bool : to generate DOT representation of dependency graph
    //! @param isWarn bool : enable or disable warnings
    //! @param stream std::ostream& : stream to print errors and warnings
    KIF(const std::string& output_filename, bool saveGraph = false, bool isWarn = true,
        std::ostream& stream = std::cerr)
        : output_filename(output_filename), facts(), clauses(),
        driver(stream, *this, saveGraph, isWarn)
        { }

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

    //! getter setter for output filename
    const std::string& OutputFilename() const { return output_filename; }
    std::string& OutputFilename() { return output_filename; }

    //! getter setter for graph output filename
    const std::string& GraphFilename() const { return graph_filename; }
    std::string& GraphFilename() { return graph_filename; }

    //! get facts
    std::vector<Fact> Facts() { return facts; }
    //! get clauses
    std::vector<Clause> Clauses() { return clauses; }

private:
    //! make KIFDriver class friend
    friend KIFDriver;

    //! add fact and clause to this kif -- used by KIFDriver
    void AddFact(const Fact& f) { facts.push_back(f); }
    void AddClause(const Clause& c) { clauses.push_back(c); }


    //! filename of output file
    std::string output_filename;
    //! filename of graph output
    std::string graph_filename;

    //! All the facts
    std::vector<Fact> facts;

    //! All the clauses
    std::vector<Clause> clauses;

    //! driver to drive parsing
    KIFDriver driver;
};

} //namespace gdlparser

#endif // _GDLPARSER_KIF_HPP_INCLUDED
