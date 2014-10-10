/**
 * @file core.hpp
 * @author Sumedh Ghaisas
 *
 * Includes all the base components.
 */
#ifndef __LIBGDL_CORE_HPP
#define __LIBGDL_CORE_HPP

#include <libgdl/core/data_types/argument.hpp>
#include <libgdl/core/data_types/clause.hpp>
#include <libgdl/core/data_types/fact.hpp>
#include <libgdl/core/data_types/state.hpp>
#include <libgdl/core/data_types/location.hpp>
#include <libgdl/core/data_types/move.hpp>
#include <libgdl/core/data_types/intrusive_list.hpp>
#include <libgdl/core/data_types/variable_map.hpp>

#include <libgdl/core/dgraph/dgraph_node.hpp>
#include <libgdl/core/symbol_table/symbol_decode_stream.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>

#include <libgdl/core/util/setop.hpp>
#include <libgdl/core/util/logid.hpp>
#include <libgdl/core/util/log.hpp>
#include <libgdl/core/util/timer.hpp>
#include <libgdl/core/util/to_string.hpp>
#include <libgdl/core/util/version.hpp>

#define MARK_USED(X) (void)X;

#endif // __LIBGDL_CORE_HPP
