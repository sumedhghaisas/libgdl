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
#include <boost/unordered_map.hpp>

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
        std::ostream* stream = &std::cout)
        : stream(stream), isWarn(isWarn),
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
        files.push_back(filename);
    }
    void AddFile(const std::vector<std::string>& fvec)
    {
        for(size_t i = 0;i < fvec.size();i++) files.push_back(fvec[i]);
    }

    const std::vector<std::string>& Files() const
    {
        return files;
    }

    std::vector<std::string>& Files()
    {
        return files;
    }

    void ClearFiles()
    {
        files.clear();
    }

    void AddLineMark(const location_type& loc);

    //! Parse the inputs
    //!
    //! @return bool : success or failure
    //!
    bool Parse(bool ignoreErrors = false)
    {
        bool res = driver.Parse();
        if(!res && !ignoreErrors)
        {
            facts.clear();
            clauses.clear();
        }
        return res;
    }

    bool DeepScan();

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

    //! get the dependency graph
    const std::map<std::string, DGraphNode*>& DependencyGraph() const
    {
        return dgraph;
    }

    bool DebuggingSymbolSupport() const
    {
        return isDebuggingSymbols;
    }

private:
    //! make KIFDriver class friend
    friend KIFDriver;

    //! add fact and clause to this kif -- used by KIFDriver
    void AddFact(const Fact& f, const location_type& loc)
    {
        facts.push_back(f);
        facts[facts.size() - 1].AddLocation(loc);
    }
    void AddClause(const Clause& c, const location_type& loc)
    {
        clauses.push_back(c);
        clauses[clauses.size() - 1].AddLocation(loc);
    }

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

    std::vector<std::string> files;

    //! driver to drive parsing
    KIFDriver driver;

    //! index of the last fact tagged with "#line" location
    size_t f_last_index_with_linemark;
    //! index of the last clause tagged with "#line" location
    size_t c_last_index_with_linemark;

    std::vector<std::string> roles;
};

} //namespace gdlparser

#endif // _GDLPARSER_KIF_HPP_INCLUDED
