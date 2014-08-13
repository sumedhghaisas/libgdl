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
typedef libgdl::gdlparser::parser::yy::KIFParser::token token;
typedef libgdl::gdlparser::parser::yy::KIFParser::symbol_type symbol_type;
typedef libgdl::gdlparser::parser::yy::KIFParser::location_type location_type;
typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

#define IN_MAKE_FIRST std::string(yytext)
#define IN_MAKE_SECOND location_type(&files[file_index - 1], lineNo + 1, charNo)
#define IN_MAKE IN_MAKE_FIRST, IN_MAKE_SECOND

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return KIFParser::make_END(IN_MAKE_SECOND);

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "ExampleFlexLexer" */
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
#define YY_USER_ACTION  charNo = charNo + yyleng;
%}

%% /*** Regular Expressions Part ***/

"(" {
    return KIFParser::make_Obracket(IN_MAKE);
}
")" {
    return KIFParser::make_Cbracket(IN_MAKE);
}
"role" {
    return KIFParser::make_role(IN_MAKE);
}
"base" {
    return KIFParser::make_base(IN_MAKE);
}
"input" {
    return KIFParser::make_input(IN_MAKE);
}
"init" {
    return KIFParser::make_init(IN_MAKE);
}
"legal" {
    return KIFParser::make_legal(IN_MAKE);
}
"goal" {
    return KIFParser::make_goal(IN_MAKE);
}
"terminal" {
    return KIFParser::make_terminal(IN_MAKE);
}
"next" {
    return KIFParser::make_next(IN_MAKE);
}
"or" {
    return KIFParser::make_oor(IN_MAKE);
}
"not" {
    return KIFParser::make_nnot(IN_MAKE);
}
"true" {
    return KIFParser::make_ttrue(IN_MAKE);
}
"distinct" {
    return KIFParser::make_distinct(IN_MAKE);
}
"does" {
    return KIFParser::make_does(IN_MAKE);
}
"<=" {
    return KIFParser::make_clause_command(IN_MAKE);
}
[;].*\n {
    lineNo++;
    charNo = 0;
    std::string comment(yytext);
    if(comment.find("#line")!= std::string::npos) return KIFParser::make_hash_line(IN_MAKE);
}
[0-9]+ {
    return KIFParser::make_num(IN_MAKE);
}
[^()[:blank:]\n]+ {
    if(yytext[0] == '?') return KIFParser::make_var(IN_MAKE);
    return KIFParser::make_id(IN_MAKE);
}
[[:blank:]] {
}
[\n] {
    lineNo++;
    charNo = 0;
}

%% /*** Additional Code ***/


/* This implementation of ExampleFlexLexer::yylex() is required to fill the
 * vtable of the class ExampleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int libgdl::gdlparser::parser::KIFScanner::yywrap()
{
    // if no file to scan
    if(files.size() == 0)
    {
        driver.Warn("No file provided...");
        return 1;
    }
    // choose file file to scan
    else if(file_index < files.size())
    {
        std::ifstream* temp = new std::ifstream(files[file_index].c_str());
        // if invalid file
        ASSERT(TEMP->is_open(), "Could not open file " + files[file_index]);
        delete yyin;
        yyin = temp;
    }
    else return 1;
    // increment file index
    file_index++;
    return 0;
}

libgdl::gdlparser::parser::KIFScanner::KIFScanner(const KIFDriver& driver)
        : yyFlexLexer(new std::stringstream(), NULL),
        driver(driver), files(driver.files),
        file_index(0)
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
