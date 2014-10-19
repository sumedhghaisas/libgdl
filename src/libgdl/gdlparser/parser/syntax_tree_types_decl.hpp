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

class Node;

class TerminalHold;

template<class Policy, class Head, class Hold>
class BasicConstruct;

struct FunctionPolicy;
struct RelationPolicy;
struct ClausePolicy;

typedef BasicConstruct<FunctionPolicy, std::string, TerminalHold> Term;
typedef BasicConstruct<RelationPolicy, std::string, Term> Sentence;
typedef BasicConstruct<RelationPolicy, std::string, Sentence> Premiss;
typedef BasicConstruct<ClausePolicy, Sentence, Premiss> ClauseConstruct;

}
}
}


#endif // _LIBGDL_GDLPARSER_PARSER_SYNTAX_TREE_TYPES_HPP_INCLUDED
