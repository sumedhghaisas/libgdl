/**
 * @file kif_scanner.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration scanner class for Game Description Language.
 */
#ifndef _GDLPARSER_PARSER_KIF_SCANNER_HPP_INCLUDED
#define _GDLPARSER_PARSER_KIF_SCANNER_HPP_INCLUDED

#include <libgdl/core.hpp>
#include <libgdl/core/util/gdl_stream.hpp>

#include "kif_parser.tab.hh"

#include <iostream>
#include <sstream>
#include <fstream>

#ifndef YY_DECL

#define	YY_DECL						\
    libgdl::gdlparser::parser::yy::KIFParser::token_type				\
    libgdl::gdlparser::parser::KIFScanner::lex(				\
	libgdl::gdlparser::parser::yy::KIFParser::semantic_type* yylval,		\
	libgdl::gdlparser::parser::yy::KIFParser::location_type* yylloc		\
    )
#endif

#include "FlexLexer.h"

namespace libgdl
{
namespace gdlparser
{
namespace parser
{
//! forward declaration of class KIFParser
class KIFDriver;

/**
 * This class implements tokenizer for Game Description Language. Implements yylex
 * method used by KIFParser which returns next token. Scanner can handle multiple
 * files and returns location along with token for errors and warnings.
 *
 */
class KIFScanner : public yyFlexLexer
{
    //! some useful typedefs
    typedef gdlparser::parser::yy::KIFParser::semantic_type semantic_type;
    typedef gdlparser::parser::yy::KIFParser::token_type token_type;
    typedef gdlparser::parser::yy::KIFParser::location_type location_type;

    //! to store state of the scanner
    enum State { NoState, InCommentFromToken, InComment, InToken, InClauseCommand, HaveNextToken};

public:

    //! Constructor
    //! Reference to the calling parser class is stored for error and warning logging
    KIFScanner(KIFDriver& driver);

    ~KIFScanner() { delete yyin; }

    //! yylex function needed by parser.
    //! returns the next token if any along with its location
    token_type lex(semantic_type* yylval,
                   location_type* yylloc);

    //! current location of the scanner
    int LineNo() { return lineNo + 1; }
    int CharNo() { return charNo; }
    std::string& CurrentFile() { return streams[stream_index].Name(); }

    void set_debug(bool b)
    {
        yy_flex_debug = b;
    }

    int yywrap();

private:
    //! empty stringstream
    std::stringstream empty_stringstream;

    //! reference of the calling driver object
    KIFDriver& driver;

    //! files to be scanned
    std::vector<util::GDLStream>& streams;

    //! store next token to be returned
    std::string nextTokenValue;

    //! current state of the scanner
    State state;

    //! to track the location of the token
    int lineNo;
    int charNo;

    //! current file index
    size_t stream_index;
};

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _GDLPARSER_PARSER_KIF_SCANNER_HPP_INCLUDED
