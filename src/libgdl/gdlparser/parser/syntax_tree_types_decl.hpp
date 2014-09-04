/**
 * @file syntax_tree_types_decl.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration Syntax Tree Types.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_SYNTAX_TREE_TYPES_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_SYNTAX_TREE_TYPES_HPP_INCLUDED

namespace libgdl
{
namespace gdlparser
{
namespace parser
{

class TerminalHold;

template<class Policy, class Head, class Hold>
class BasicConstruct;

class FunctionPolicy;

class Node;

typedef BasicConstruct<FunctionPolicy, size_t, TerminalHold> Term;

}
}
}


#endif // _LIBGDL_GDLPARSER_PARSER_SYNTAX_TREE_TYPES_HPP_INCLUDED
