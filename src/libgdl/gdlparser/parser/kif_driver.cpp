/**
 * @file kif_driver.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation driver class.
 */
#include "kif_driver.hpp"

#include <cctype>
#include <fstream>
#include <cassert>
#include <fstream>

#include <libgdl/gdlparser/kif.hpp>

// Color code escape sequences
#define BASH_RED "\033[0;31m"
#define BASH_YELLOW "\033[0;33m"
#define BASH_CLEAR "\033[0m"

using namespace std;
using namespace libgdl;
using namespace libgdl::util;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlparser::parser;

KIFDriver::KIFDriver(KIF& kif)
    : kif(kif),
    streams(kif.streams)
{
  scanner = new KIFScanner(*this);
  parser = NULL;

  anyError = false;

  current_id = 0;
}

KIFDriver::~KIFDriver()
{
  delete(scanner);
  scanner = NULL;
  delete(parser);
  parser = NULL;

  for(std::list<std::string*>::iterator it = to_free.begin();it != to_free.end();it++)
    delete *it;
}

const SymbolTable* KIFDriver::GetSymbolTable() const
{
  return kif.symbol_table;
}

SymbolTable* KIFDriver::GetSymbolTable()
{
  return kif.symbol_table;
}

const DGraph* KIFDriver::GetDGraph() const
{
  return kif.dgraph;
}

DGraph* KIFDriver::GetDGraph()
{
  return kif.dgraph;
}

void KIFDriver::Error(const ErrorType& error)
{
  kif.errors.push_back(error);
}

void KIFDriver::Warn(const ErrorType& warn)
{
  kif.warnings.push_back(warn);
}

bool KIFDriver::Parse()
{
    // create a new parser object
    parser = new yy::KIFParser( (*scanner), (*this));

    // perform syntactical stage of parsing
    if( parser->parse() != 0 || anyError == true)
    {
        // if failed
        return false;
    }

//    //! check if role, terminal, goal and legal relations are defined
//    std::vector<std::string> c_rels;
//    c_rels.push_back("role");
//    c_rels.push_back("terminal");
//    c_rels.push_back("goal");
//    c_rels.push_back("legal");
//    for(size_t i = 0; i < c_rels.size(); i++)
//    {
//        std::map<std::string, Symbol>::const_iterator it = symbol_table.find(c_rels[i]);
//        if(it == symbol_table.end() || (it->second).isDefined == false)
//            Error("No '" + c_rels[i] + "' relations found in the input.");
//    }

    // check for stratification
    //CheckCycles();

    // check that relation init, base and input are not dependent on true or does
    // check that relation legal is not dependent on relation legal
    //CheckRecursiveDependencies();

//    // show undefiend symbol warnings
//    for(std::map<std::string, Symbol>::iterator it = symbol_table.begin(); it != symbol_table.end(); it++)
//    {
//        if((it->second).isDefined == false) Warn((it->second).first_occurrence, "Undefined symbol " + (it->second).name);
//    }

  if(anyError)
  {
    return false;
  }

  return true;
}

void KIFDriver::AddLineMark(const TokenValue& tok)
{
    // get value from the tken
    const std::string& val = tok.Value();

    size_t start = val.find("#line");

    // get line number
    // if invalid return
    size_t i = val.find(" ", start + 1);
    if(i == std::string::npos) return;
    size_t j = val.find(" ", i + 1);
    if(j == std::string::npos) return;
    std::string str_lineNo = val.substr(i + 1, j - i - 1).c_str();

    for (size_t i = 0; i < str_lineNo.length(); i++)
    {
        if (!isdigit(str_lineNo[i])) return;
    }
    size_t lineNo = atoi(str_lineNo.c_str());

    // get filename
    // if none provided assume it to be ""
    i = val.find("\"");
    std::string filename = "";
    if(i != std::string::npos)
    {
        j = val.find("\"", i + 1);
        if(j == std::string::npos) return;
        filename = val.substr(i + 1, j - i - 1);
    }

    std::string* temp = new std::string(filename);

    to_free.push_back(temp);

    // process facts and clauses accordingly
    kif.AddLineMark(location_type(temp, lineNo, 0));
    return;

}

const Fact& KIFDriver::AddFact(Fact&& f_t)
{
    return kif.AddFact(std::move(f_t));
}

const Clause& KIFDriver::AddClause(Clause&& c_t)
{
    return kif.AddClause(std::move(c_t));
}

void KIFDriver::CheckDef15(size_t c_index, const Argument& arg, const std::set<DGraphNode*>& scc,
                           const location_type& loc)
{
//    const Clause& c = kif.Clauses()[c_index];
//
//    const std::vector<Argument*>& premisses = c.premisses;
//
//    // find the index of the given argument in the clause
//    size_t arg_index = 0;
//    for(size_t i = 0;i < premisses.size();i++)
//        if(premisses[i]->OrEquate(arg))
//        {
//            arg_index = i;
//            break;
//        }
//
//    // check for each argument in given argument
//    bool isValid = true;
//    size_t invalid_index = 0;
//    for(size_t i = 0;i < arg.args.size();i++)
//    {
//        // check if the argument is ground
//        if(arg.args[i]->IsGround()) continue;
//        // check if this argument is also argument to head
//        if(c.head->HasAsArgument(*(arg.args[i]))) continue;
//
//        bool isFound = false;
//
//        for(size_t j = 0;j < premisses.size();j++)
//        {
//            // avoid checking in itself
//            if(j == arg_index) continue;
//
//            // if another premiss has same same argument and is not in the same SCC
//            if(premisses[j]->HasAsArgument(*(arg.args[i])) && scc.find(dgraph[premisses[j]->val]) == scc.end())
//            {
//                isFound = true;
//                break;
//            }
//        }
//
//        if(!isFound)
//        {
//            isValid = false;
//            invalid_index = i;
//            break;
//        }
//    }
//
//    if(!isValid)
//    {
//        std::stringstream stream;
//        stream << *arg.args[invalid_index];
//        Error(loc, "Unstratified Recursion: Relation involved in the cycle is " + arg.val +
//                       ". Restriction violated for variable " + stream.str());
//    }

}


