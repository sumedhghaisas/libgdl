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
  scanner = NULL;
  parser = NULL;
}

KIFDriver::~KIFDriver()
{
  delete(scanner);
  scanner = NULL;
  delete(parser);
  parser = NULL;

  for(list<string*>::iterator it = to_free.begin();it != to_free.end();it++)
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
  delete scanner;
  delete parser;
  scanner = new KIFScanner(*this);
  // create a new parser object
  parser = new yy::KIFParser(*scanner, *this);

  // perform syntactical stage of parsing
  if(parser->parse() != 0)
    return false;
  return true;
}

const Fact& KIFDriver::AddFact(Fact&& f_t)
{
    return kif.AddFact(std::move(f_t));
}

const Clause& KIFDriver::AddClause(Clause&& c_t)
{
    return kif.AddClause(std::move(c_t));
}
