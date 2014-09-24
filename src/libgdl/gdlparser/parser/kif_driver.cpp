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
using namespace libgdl::core;
using namespace libgdl::util;
using namespace libgdl::gdlparser;
using namespace libgdl::gdlparser::parser;

KIFDriver::KIFDriver(KIF& kif)
    : kif(kif),
    streams(kif.streams)
{
  scanner = new KIFScanner(*this);
  parser = NULL;

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

const SymbolTable& KIFDriver::GetSymbolTable() const
{
  return kif.symbol_table;
}

SymbolTable& KIFDriver::GetSymbolTable()
{
  return kif.symbol_table;
}

const DGraph& KIFDriver::GetDGraph() const
{
  return kif.dgraph;
}

DGraph& KIFDriver::GetDGraph()
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
  if(parser->parse() != 0)
    return false;
  return true;
}

//void KIFDriver::AddLineMark(const TokenValue& tok)
//{
//    // get value from the tken
//    const std::string& val = tok.Value();
//
//    size_t start = val.find("#line");
//
//    // get line number
//    // if invalid return
//    size_t i = val.find(" ", start + 1);
//    if(i == std::string::npos) return;
//    size_t j = val.find(" ", i + 1);
//    if(j == std::string::npos) return;
//    std::string str_lineNo = val.substr(i + 1, j - i - 1).c_str();
//
//    for (size_t i = 0; i < str_lineNo.length(); i++)
//    {
//        if (!isdigit(str_lineNo[i])) return;
//    }
//    size_t lineNo = atoi(str_lineNo.c_str());
//
//    // get filename
//    // if none provided assume it to be ""
//    i = val.find("\"");
//    std::string filename = "";
//    if(i != std::string::npos)
//    {
//        j = val.find("\"", i + 1);
//        if(j == std::string::npos) return;
//        filename = val.substr(i + 1, j - i - 1);
//    }
//
//    std::string* temp = new std::string(filename);
//
//    to_free.push_back(temp);
//
//    // process facts and clauses accordingly
//    kif.AddLineMark(location_type(temp, lineNo, 0));
//    return;
//}

const Fact& KIFDriver::AddFact(Fact&& f_t)
{
    return kif.AddFact(std::move(f_t));
}

const Clause& KIFDriver::AddClause(Clause&& c_t)
{
    return kif.AddClause(std::move(c_t));
}
