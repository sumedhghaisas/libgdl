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
 * This class implements scanner for GDL. The implementation of the scanner
 * (yylex) is done by flex. For more information about the scanner check out the
 * file kif_scanner.ll.
 */
class KIFScanner : public yyFlexLexer
{
    //! some useful imports
    typedef gdlparser::parser::yy::KIFParser::semantic_type semantic_type;
    typedef gdlparser::parser::yy::KIFParser::token_type token_type;
    typedef gdlparser::parser::yy::KIFParser::location_type location_type;

public:

    //! Empty constructor
    //! Reference to the calling parser class is stored for error and warning
    //! logging
    //!
    //! \param driver Reference to the calling driver
    //!
    //!
    KIFScanner(KIFDriver& driver);

    //! Destructor
    ~KIFScanner() { delete yyin; }

    //! yylex function needed by parser.
    //! Returns the next token if any along with its location
    //! For implementation check out kif_scanner.ll
    //!
    //! \param yyval Value of the token
    //! \param yyloc Location of the token
    //! \return token_type
    //!
    //!
    token_type lex(semantic_type* yylval,
                   location_type* yylloc);

    //! Get current stream name
    std::string& CurrentFile() { return streams[stream_index].Name(); }

    //! Set debug
    void set_debug(bool b)
    {
        yy_flex_debug = b;
    }

    //! Wrap function used by the scanner
    //! This function called the current stream ends
    int yywrap();

private:
    //! Empty stringstream
    std::stringstream empty_stringstream;

    //! reference of the calling driver object
    KIFDriver& driver;

    //! files to be scanned
    std::vector<util::GDLStream>& streams;

    //! current file index
    size_t stream_index;
}; // class KIFScanner

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _GDLPARSER_PARSER_KIF_SCANNER_HPP_INCLUDED
