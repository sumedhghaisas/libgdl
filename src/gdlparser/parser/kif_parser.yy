/**
 * @file kif_parser.yy
 * @author Sumedh Ghaisas
 *
 * Bison file for generating GDL parser.
 */

%debug
%language "c++"
%defines
/* parser will be wrapped in namepsace gdlparser::parser::yy */
%define api.namespace {gdlparser::parser::yy}
/* class name of parser will be KIFParser. */
%define parser_class_name {KIFParser}
%define api.token.constructor
/* for user defined value types. */
%define api.value.type variant
/* Generate verbose errors. */
%error-verbose
/* Support location tracking. */
%locations

%code requires // *.hh
{
#include <list>
#include <string>

// custom value type
#include "token_value.hpp"

typedef std::list<std::string> strings_type;
typedef gdlparser::parser::TokenValue TokenValue;

namespace gdlparser {
    namespace parser {
            class KIFDriver;
            class KIFScanner;
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

// use yylex function in scanner class instead of predefiend yylex
#undef yylex
#define yylex scanner.yylex

}

/* Terminals */

%token          END     0       "end of file"
%token  <TokenValue>  Obracket  "opening bracket"
%token  <TokenValue>  Cbracket  "closing bracket"
%token  <TokenValue>  id        "symbol"
%token  <TokenValue>  var       "variable"
%token  <TokenValue>  num       "number"
%token  <TokenValue>  role      "role"
%token  <TokenValue>  input     "input"
%token  <TokenValue>  init      "init"
%token  <TokenValue>  base      "base"
%token  <TokenValue>  legal     "legal"
%token  <TokenValue>  next      "next"
%token  <TokenValue>  goal      "goal"
%token  <TokenValue>  terminal  "terminal"
%token  <TokenValue>  does      "does"
%token  <TokenValue>  ttrue     "true"
%token  <TokenValue>  nnot      "not"
%token  <TokenValue>  oor       "or"
%token  <TokenValue>  distinct  "distinct"
%token  <TokenValue>  clause_command    "clause command"

/* Nonterminals */

%type   <TokenValue>   Start
%type   <TokenValue>    S
%type   <TokenValue>  Fact
%type   <TokenValue>  Clause
%type   <TokenValue>  Ground_term
%type   <TokenValue>  Ground_terms
%type   <TokenValue>  Id_num
%type   <TokenValue>  Head
%type   <TokenValue>  ORelation
%type   <TokenValue>  ORelations
%type   <TokenValue>  Term
%type   <TokenValue>  Terms
%type   <TokenValue>  Num_var
%type   <TokenValue>  CTerm
%type   <TokenValue>  CTerms
%type   <TokenValue>  Literal

%%

/* Grammer */

Start       : S END

S           : Fact S    {
                            driver.AddFact($1, @1); $2;
                        }
            | Clause S  {
                            driver.AddClause($1, @1); $2;
                        }
            | %empty    { }

Fact        : Obracket role Id_num Cbracket         {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.Command() = $2.Value();
                                                        $$.AddArgument($3);
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Obracket base Ground_term Cbracket    {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.Command() = $2.Value();
                                                        $$.AddArgument($3);
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Obracket input Ground_term Ground_term Cbracket   {
                                                                    $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                                    $$.Type() = TokenValue::Relation;
                                                                    $$.Command() = $2.Value();
                                                                    $$.AddArgument($3);
                                                                    $$.AddArgument($4);
                                                                    std::string msg;
                                                                    int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                                    if(res == 1) error(@2, msg);
                                                                    else if(res == 2) driver.Warn(@2, msg);
                                                                }
            | Obracket init Ground_term Cbracket    {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.Command() = $2.Value();
                                                        $$.AddArgument($3);
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Obracket legal Ground_term Ground_term Cbracket   {
                                                                    $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                                    $$.Type() = TokenValue::Relation;
                                                                    $$.Command() = $2.Value();
                                                                    $$.AddArgument($3);
                                                                    $$.AddArgument($4);
                                                                    std::string msg;
                                                                    int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                                    if(res == 1) error(@2, msg);
                                                                    else if(res == 2) driver.Warn(@2, msg);
                                                                }
            | Obracket next Ground_term Cbracket    {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.Command() = $2.Value();
                                                        $$.AddArgument($3);
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Obracket goal Ground_term num Cbracket    {
                                                            $4.Command() = $4.Value();
                                                            $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                            $$.Type() = TokenValue::Relation;
                                                            $$.Command() = $2.Value();
                                                            $$.AddArgument($3);
                                                            $$.AddArgument($4);
                                                            std::string msg;
                                                            int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                            if(res == 1) error(@2, msg);
                                                            else if(res == 2) driver.Warn(@2, msg);

                                                        }
            | terminal  {
                            $$ = $$ + $1;
                            $$.Type() = TokenValue::Relation;
                            $$.Command() = $1.Value();
                            std::string msg;
                            int res = driver.AddEntry($$, true, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }
            | Obracket Id_num Ground_term Ground_terms Cbracket {
                                                                    $$ = $1 + " " + $2 + " " + $3;
                                                                    if($4 != "") $$ = $$ + " " + $4;
                                                                    $$ = $$ + " " + $5;
                                                                    $$.Type() = TokenValue::Relation;
                                                                    $$.Command() = $2.Value();
                                                                    $$.AddArgument($3);
                                                                    $$.AddArguments($4.Arguments());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                                    if(res == 1) error(@2, msg);
                                                                    else if(res == 2) driver.Warn(@2, msg);
                                                                }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Type() = TokenValue::Relation;
                            $$.Command() = $1.Value();
                            std::string msg;
                            int res = driver.AddEntry($$, true, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }

Ground_term : Obracket Id_num Ground_term Ground_terms Cbracket {
                                                                    $$ = $1 + " " + $2 + " " + $3;
                                                                    if($4 != "") $$ = $$ + " " + $4;
                                                                    $$ = $$ + " " + $5;
                                                                    $$.Type() = TokenValue::Function;
                                                                    $$.Command() = $2.Value();
                                                                    $$.AddArgument($3);
                                                                    $$.AddArguments($4.Arguments());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry($2, false, $$.Count(), @2, msg);
                                                                    if(res == 1) error(@2, msg);
                                                                    else if(res == 2) driver.Warn(@2, msg);
                                                                }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Type() = TokenValue::Function;
                            $$.Command() = $1.Value();
                            std::string msg;
                            int res = driver.AddEntry($$, false, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }

Ground_terms : Ground_term Ground_terms {
                                            $$ = $$ + $1;
                                            if($2.Value() != "") $$ = $$ + " " + $2;
                                            $$.Command() = "multiple terms";
                                            $$.AddArgument($1);
                                            $$.AddArguments($2.Arguments());
                                        }
            | %empty    { }

Id_num      : id    {
                        $$ = $$ + $1;
                        $$.Command() = $1.Value();
                    }
            | num   {
                        $$ = $$ + $1;
                        $$.Command() = $1.Value();
                    }

Clause      : Obracket clause_command Head ORelation ORelations Cbracket    {
                                                                                $$ = $1 + " " + $2 + " " + $3.Value() + " " + $4;
                                                                                if($5 != "") $$ = $$ + " " + $5;
                                                                                $$ = $$ + " " + $6;
                                                                                $$.HeadRestrict($3);
                                                                                $$.Command() = $2.Value();
                                                                                $$.AddArgument($3);
                                                                                $$.AddArgument($4);
                                                                                $$.AddArguments($5.Arguments());
                                                                            }

Head        : Obracket role Term Cbracket   {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                                std::string msg;
                                                int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                if(res == 1) error(@2, msg);
                                                else if(res == 2) driver.Warn(@2, msg);
                                            }
            | Obracket base Term Cbracket   {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                                std::string msg;
                                                int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                if(res == 1) error(@2, msg);
                                                else if(res == 2) driver.Warn(@2, msg);
                                            }
            | Obracket input Term Term Cbracket {
                                                    $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                    $$.Command() = $2.Value();
                                                    $$.Type() = TokenValue::Relation;
                                                    $$.AddArgument($3);
                                                    $$.AddArguments($4.Arguments());
                                                    std::string msg;
                                                    int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                    if(res == 1) error(@2, msg);
                                                    else if(res == 2) driver.Warn(@2, msg);
                                                }
            | Obracket init Term Cbracket   {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                                std::string msg;
                                                int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                if(res == 1) error(@2, msg);
                                                else if(res == 2) driver.Warn(@2, msg);
                                            }
            | Obracket legal Term Term Cbracket {
                                                    $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                    $$.Command() = $2.Value();
                                                    $$.Type() = TokenValue::Relation;
                                                    $$.AddArgument($3);
                                                    $$.AddArguments($4.Arguments());
                                                    std::string msg;
                                                    int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                    if(res == 1) error(@2, msg);
                                                    else if(res == 2) driver.Warn(@2, msg);
                                                }
            | Obracket next Term Cbracket   {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                                std::string msg;
                                                int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                if(res == 1) error(@2, msg);
                                                else if(res == 2) driver.Warn(@2, msg);
                                            }
            | Obracket goal Term Num_var Cbracket   {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | terminal  {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                            std::string msg;
                            int res = driver.AddEntry($1, true, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }
            | Obracket Id_num Term Terms Cbracket   {
                                                        $$ = $1 + " " + $2 + " " + $3;
                                                        if($4 != "") $$ = $$ + " " + $4;
                                                        $$ = $$ + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, true, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                            $$.Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.AddEntry($1, true, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }

ORelation   : Literal   { $$ = $1; }
            | Obracket nnot Literal Cbracket    {
                                                    $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                    $$.PerformNot();
                                                    $$.Command() = $2.Value();
                                                    $$.Type() = TokenValue::Relation;
                                                    $$.AddArgument($3);
                                                }
ORelations  : ORelation ORelations  {
                                        $$ = $$ + $1;
                                        if($2 != "") $$ = $$ + " " + $2;
                                        $$.Command() = "Multiple relations";
                                        $$.AddArgument($1);
                                        $$.AddArguments($2.Arguments());
                                    }
            | %empty    { }


Literal     : Obracket does CTerm CTerm Cbracket    {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                    }
            | Obracket ttrue CTerm Cbracket         {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                    }
            | Obracket oor ORelation ORelations Cbracket    {
                                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4.Value() + " " + $5;
                                                                $$.OrRestrict($4);
                                                                $$.Command() = $2.Value();
                                                                $$.Type() = TokenValue::Relation;
                                                                $$.AddArgument($3);
                                                                $$.AddArguments($4.Arguments());
                                                            }
            | Obracket distinct CTerm CTerm Cbracket    {
                                                            $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                            $$.PerformNot();
                                                            $$.Command() = $2.Value();
                                                            $$.Type() = TokenValue::Relation;
                                                            $$.AddArgument($3);
                                                            $$.AddArguments($4.Arguments());
                                                        }
            | Obracket role CTerm Cbracket  {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                            }
            | Obracket base CTerm Cbracket  {
                                                $$ = $1 + " " + $2 + " " + $3 + " " + $4;
                                                $$.Command() = $2.Value();
                                                $$.Type() = TokenValue::Relation;
                                                $$.AddArgument($3);
                                            }
            | Obracket input CTerm CTerm Cbracket   {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                    }
            | Obracket legal CTerm CTerm Cbracket   {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                    }
            | Obracket goal CTerm Num_var Cbracket  {
                                                        $$ = $1 + " " + $2 + " " + $3 + " " + $4 + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Relation;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                    }
            | terminal  {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                        }
            | Obracket Id_num CTerm CTerms Cbracket     {
                                                            $$ = $1 + " " + $2 + " " + $3;
                                                            if($4 != "") $$ = $$ + " " + $4;
                                                            $$ = $$ + " " + $5;
                                                            $$.Command() = $2.Value();
                                                            $$.Type() = TokenValue::Relation;
                                                            $$.AddArgument($3);
                                                            $$.AddArguments($4.Arguments());
                                                            std::string msg;
                                                            int res = driver.CheckEntry($2, true, $4.Count() + 1, @2, msg);
                                                            if(res == 1) error(@2, msg);
                                                            else if(res == 2) driver.Warn(@2, msg);
                                                        }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                            $$.Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.CheckEntry($1, true, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }


Term        : Obracket Id_num Term Terms Cbracket   {
                                                        $$ = $1 + " " + $2 + " " + $3;
                                                        if($4 != "") $$ = $$ + " " + $4;
                                                        $$ = $$ + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Function;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry($2, false, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                            $$.Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.AddEntry($1, false, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }
            | var   {
                        $$ = $$ + $1;
                        $$.AddVariable($1);
                        $$.Command() = $1.Value();
                        $$.Type() = TokenValue::Var;
                    }

Terms       : Term Terms    {
                                $$ = $$ + $1;
                                if($2 != "") $$ = $$ + " " + $2;
                                $$.Command() = "Multiple terms";
                                $$.AddArgument($1);
                                $$.AddArguments($2.Arguments());
                            }

            | %empty    { }

CTerm       : Obracket Id_num CTerm CTerms Cbracket {
                                                        $$ = $1 + " " + $2 + " " + $3;
                                                        if($4 != "") $$ = $$ + " " + $4;
                                                        $$ = $$ + " " + $5;
                                                        $$.Command() = $2.Value();
                                                        $$.Type() = TokenValue::Function;
                                                        $$.AddArgument($3);
                                                        $$.AddArguments($4.Arguments());
                                                        std::string msg;
                                                        int res = driver.CheckEntry($2, false, $$.Count(), @2, msg);
                                                        if(res == 1) error(@2, msg);
                                                        else if(res == 2) driver.Warn(@2, msg);
                                                    }
            | Id_num    {
                            $$ = $$ + $1;
                            $$.Command() = $1.Value();
                            $$.Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.CheckEntry($1, false, 0, @1, msg);
                            if(res == 1) error(@1, msg);
                            else if(res == 2) driver.Warn(@1, msg);
                        }
            | var   {
                        $$ = $$ + $1;
                        $$.AddVariable($1);
                        $$.Command() = $1.Value();
                        $$.Type() = TokenValue::Var;
                    }

CTerms      : CTerm CTerms  {
                                $$ = $$ + $1;
                                if($2.Value() != "") $$ = $$ + " " + $2;
                                $$.Command() = "Multiple terms";
                                $$.Type() = TokenValue::Function;
                                $$.AddArgument($1);
                                $$.AddArguments($2.Arguments());
                            }

            | %empty    { }

Num_var     : num   {
                        $$ = $$ + $1;
                        $$.Command() = $1.Value();
                        $$.Type() = TokenValue::Function;
                    }
            | var   {
                        $$ = $$ + $1;
                        $$.AddVariable($1);
                        $$.Type() = TokenValue::Var;
                    }

%%

  typedef gdlparser::parser::yy::KIFParser KIFParser;

  // Mandatory error function
  void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
  {
    driver.Error(loc, msg);
  }
