/**
 * @file kif_parser.yy
 * @author Sumedh Ghaisas
 *
 * Bison file for generating GDL parser.
 */
/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start start

%language "c++"

/* write out a header file containing the token defines */
%defines

/* parser will be wrapped in namepsace gdlparser::parser::yy */
%define api.namespace {libgdl::gdlparser::parser::yy}
/* class name of parser will be KIFParser. */
%define parser_class_name {KIFParser}

/* Generate verbose errors. */
%error-verbose

/* Support location tracking. */
%locations

%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &scanner.CurrentFile();
};

%union {
    size_t                      num;
    std::string*                stringVal;
    Node*                       node;
}

%code requires // *.hh
{
#include <list>
#include <string>

#include "syntax_tree_types_decl.hpp"

namespace libgdl {
namespace gdlparser {
    namespace parser {
            class KIFDriver;
            class KIFScanner;
        }
    }
  }
}

/* parameters of KIFParser constructor. */
%parse-param { KIFScanner  &scanner  }
%parse-param { KIFDriver  &driver  }

%code // *.cc
{
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

// driver class declaration.
#include "kif_driver.hpp"

#include "syntax_tree_types_includes.hpp"
#include <libgdl/core/symbol_table/symbol_table.hpp>

// use yylex function in scanner class instead of predefiend yylex
#undef yylex
#define yylex scanner.lex

}

/* Terminals */

%token                END  0    "end of file"
%token                OBRACKET  "opening bracket"
%token                CBRACKET  "closing bracket"
%token  <stringVal>   ID        "symbol"
%token  <stringVal>   VARIABLE  "variable"
%token  <num>         NUMBER    "number"
%token                TRUE      "true"
%token                NOT       "not"
%token                OR        "or"
%token                CCOMMAND  "clause command"
%token  <stringVal>   HLINE     "line mark"

/* Nonterminals */

%type   <node>        start
%type   <node>        S

%%

/* Grammer */

start : S   {
              std::cout << "yolo" << std::endl;
            }
S     : ID  {
              Symbol* sym = new FunctionSymbol(*$1, 0, @1);
              size_t* id = new size_t(driver.sym_table.AddEntry(*$1, sym));
              $$ = new Term(id, @1);
            }

%%

typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

// Mandatory error function
void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
{
  driver.Error(loc, msg);
}
