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

    ClauseConstruct*            clause;

    Premiss*                    premiss;
    std::list<Premiss*>*        premisses;

    Sentence*                   sentence;
    std::list<Sentence*>*       sentences;

    Term*                       term;
    std::list<Term*>*           terms;
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

#include "syntax_tree_types_includes.hpp"
#include <libgdl/core/util/to_string.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/variable_map.hpp>
#include <libgdl/core/data_types/error_type.hpp>

// driver class declaration.
#include "kif_driver.hpp"

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

%type   <sentence>    Fact

%type   <clause>      Clause

%type   <premiss>     Premiss
%type   <premisses>   Premisses

%type   <sentence>    Sentence;
%type   <sentences>   Sentences;

%type   <term>        Term
%type   <terms>       Terms

%type   <stringVal>   Command

%%

/* Grammer */

start     : S   start {
                        VariableMap v_map;
                        $1->CodeGen(driver, v_map);
                        delete $1;
                      }
          | %empty  {
                      $$ = NULL;
                    }

S         : Fact  {
                    $$ = $1;
                  }
          | Clause  {
                      $$ = $1;
                    }

Fact      : Sentence  {
                        $$ = $1;
                      }

Clause    : OBRACKET CCOMMAND Sentence Premiss Premisses CBRACKET
                      {
                        $$ = new ClauseConstruct($3, @$);
                        $$->AddArgument($4);
                        if($5 != NULL) $$->AddArgument(*$5);
                        delete $5;
                      }

Premiss   : Sentence  {
                        $$ = new Premiss(new std::string(""), @$);
                        $$->AddArgument($1);
                      }
          | OBRACKET NOT Sentence CBRACKET
                    {
                      $$ = new Premiss(new std::string("not"), @$);
                      $$->AddArgument($3);
                    }
          | OBRACKET OR Sentence Sentences CBRACKET
                    {
                      $$ = new Premiss(new std::string("or"), @$);
                      $$->AddArgument($3);
                      if($4 != NULL) $$->AddArgument(*$4);
                      delete $4;
                    }

Premisses : Premiss Premisses {
                                if($2 == NULL)
                                  $$ = new std::list<Premiss*>();
                                else $$ = $2;
                                $$->push_front($1);
                              }
          | %empty  {
                      $$ = NULL;
                    }

Sentence  : Command {
                      $$ = new Sentence($1, @$);
                    }
          | OBRACKET Command Term Terms CBRACKET  {
                                                    $$ = new Sentence($2, @$);
                                                    $$->AddArgument($3);
                                                    if($4 != NULL)
                                                      $$->AddArgument(*$4);
                                                    delete $4;
                                                  }

Sentences : Sentence Sentences  {
                                  if($2 == NULL)
                                    $$ = new std::list<Sentence*>();
                                  else $$ = $2;
                                  $$->push_front($1);
                                }
          | %empty  {
                      $$ = NULL;
                    }

Term      : Command {
                      $$ = new Term($1, @$);
                    }
          | VARIABLE  {
                        $$ = new Term($1, @$);
                      }
          | OBRACKET Command Term Terms CBRACKET  {
                                                    $$ = new Term($2, @$);
                                                    $$->AddArgument($3);
                                                    if($4 != NULL)
                                                      $$->AddArgument(*$4);
                                                    delete $4;
                                                  }

Terms     : Term Terms  {
                          if($2 == NULL)
                            $$ = new std::list<Term*>();
                          else $$ = $2;
                          $$->push_front($1);
                        }
          | %empty  {
                      $$ = NULL;
                    }

Command   : ID  {
                  $$ = $1;
                }
          | NUMBER  {
                      $$ = new std::string(ToString($1));
                    }
%%

typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

// Mandatory error function
void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
{
  libgdl::ErrorType error;
  error.AddEntry(msg, loc);
  driver.Error(error);
}
