/**
 * @file data_types.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of data types used by driver.
 */
#ifndef __LIBGDL_GDLPARSER_DATA_TYPES_H
#define __LIBGDL_GDLPARSER_DATA_TYPES_H

#include <gdlparser/parser/location.hpp>
#include <gdlparser/parser/argument.hpp>
#include <gdlparser/parser/fact.hpp>
#include <gdlparser/parser/clause.hpp>
#include <gdlparser/parser/dgraph_node.hpp>

namespace gdlparser
{

typedef parser::Argument Argument;
typedef parser::Location Location;
typedef parser::Fact Fact;
typedef parser::Clause Clause;
typedef parser::DGraphNode DGraphNode;

}; // namespace gdlparser

#endif // __LIBGDL_GDLPARSER_DATA_TYPES_H
