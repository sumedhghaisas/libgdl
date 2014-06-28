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
#include <gdlparser/parser/kif_parser.tab.hh>

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
    typedef parser::yy::KIFParser::location_type location_type;

public:
    //! Constructor
    //!
    //! @param isWarn bool : enable or disable warnings
    //! @param stream std::ostream& : stream to print errors and warnings
    KIF(bool isWarn = true,
        bool isDebuggingSymbols = true,
        char o_level = 0,
        std::ostream& stream = std::cout)
        : stream(&stream), isWarn(isWarn),
        isDebuggingSymbols(isDebuggingSymbols),
        o_level(o_level),
        driver(*this)
    {
        f_last_index_with_linemark = 0;
        c_last_index_with_linemark = 0;
    }

    ~KIF();

    //! Add given file as input
    //!
    //! @param filename const std::string& : filename of the file to be added
    //!
    void AddFile(const std::string& filename)
    {
        driver.AddFile(filename);
    }
    void AddFile(const std::vector<std::string>& files)
    {
        for(size_t i = 0; i < files.size(); i++) AddFile(files[i]);
    }

    void AddLineMark(const location_type& loc);

    //! Parse the inputs
    //!
    //! @return bool : success or failure
    //!
    bool Parse()
    {
        return driver.Parse();
    }

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
    std::vector<Fact>& Facts()
    {
        return facts;
    }
    const std::vector<Fact>& Facts() const
    {
        return facts;
    }
    //! get clauses
    std::vector<Clause>& Clauses()
    {
        return clauses;
    }
    const std::vector<Clause> Clauses() const
    {
        return clauses;
    }

    const std::map<std::string, DGraphNode*>& DependencyGraph() const
    {
        return dgraph;
    }

private:
    //! make KIFDriver class friend
    friend KIFDriver;

    //! add fact and clause to this kif -- used by KIFDriver
    void AddFact(const Fact& f, const location_type& loc)
    {
        facts.push_back(f);
        if(isDebuggingSymbols) ds_facts.push_back(loc);
    }
    void AddClause(const Clause& c, const location_type& loc)
    {
        clauses.push_back(c);
        if(isDebuggingSymbols) ds_clauses.push_back(loc);
    }

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
    //! location of respective facts
    std::vector<location_type> ds_facts;

    //! All the clauses
    std::vector<Clause> clauses;
    //! location of respective clauses
    std::vector<location_type> ds_clauses;

    //! dependency graph
    std::map<std::string, DGraphNode*> dgraph;

    //! driver to drive parsing
    KIFDriver driver;

    //! index of the last fact tagged with "#line" location
    size_t f_last_index_with_linemark;
    //! index of the last clause tagged with "#line" location
    size_t c_last_index_with_linemark;
};

} //namespace gdlparser

#endif // _GDLPARSER_KIF_HPP_INCLUDED
