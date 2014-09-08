/**
 * @file kif_scanner.ll
 * @author Sumedh Ghaisas
 *
 * lexer for GDLParser.
 */
%{ /*** C/C++ Declarations ***/

#include <string>

#include <libgdl/core.hpp>

#include "kif_scanner.hpp"
#include "kif_driver.hpp"

/* import the parser's token type into a local typedef */
typedef libgdl::gdlparser::parser::yy::KIFParser::semantic_type semantic_type;
typedef libgdl::gdlparser::parser::yy::KIFParser::token_type token_type;
typedef libgdl::gdlparser::parser::yy::KIFParser::token token;
typedef libgdl::gdlparser::parser::yy::KIFParser::location_type location_type;
typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. */
%option prefix="yy"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput

/* enables the use of start condition stacks */
%option stack

%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

"(" {
  return token::OBRACKET;
}

")" {
  return token::CBRACKET;
}

"or" {
  return token::OR;
}

"not" {
  return token::NOT;
}

"<=" {
  return token::CCOMMAND;
}

[;].*\n {
  yylval->stringVal = new std::string(yytext, yyleng);
  std::string comment(yytext);
  if(comment.find("#line")!= std::string::npos) return token::HLINE;
}

[0-9]+ {
  yylval->num = atoi(yytext);
  return token::NUMBER;
}

[?][A-Za-z][A-Za-z0-9_]* {
  yylval->stringVal = new std::string(yytext, yyleng);
  return token::VARIABLE;
}

[A-Za-z][A-Za-z0-9_]* {
  yylval->stringVal = new std::string(yytext, yyleng);
  return token::ID;
}

 /* gobble up white-spaces */
[ \t\r]+ {
  yylloc->step();
}

 /* gobble up end-of-lines */
\n {
  yylloc->lines(yyleng); yylloc->step();
}

%% /*** Additional Code ***/


/* This implementation of ExampleFlexLexer::yylex() is required to fill the
 * vtable of the class ExampleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

#include <libgdl/core/data_types/error_type.hpp>

int libgdl::gdlparser::parser::KIFScanner::yywrap()
{
  // if no stream to scan
  if(streams.size() == 0)
  {
    core::ErrorType error;
    error.AddEntry("o streams provided for parsing", core::Location());
    driver.Warn(error);
    return 1;
  }
  // choose file file to scan
  else if(stream_index < streams.size())
  {
    yyin = streams[stream_index].Stream();
  }
  else return 1;
  // increment file index
  stream_index++;
  return 0;
}

libgdl::gdlparser::parser::KIFScanner::KIFScanner(KIFDriver& driver)
        : yyFlexLexer(&empty_stringstream, NULL),
        driver(driver), streams(driver.streams),
        stream_index(0)
{ state = NoState; lineNo = 0; charNo = 0;}


int yyFlexLexer::yylex()
{
  std::cerr << "in ExampleFlexLexer::yylex() !" << std::endl;
  return 0;
}

int yyFlexLexer::yywrap()
{
  std::cout << "in yywrap2" << std::endl;
  return 1;
}
