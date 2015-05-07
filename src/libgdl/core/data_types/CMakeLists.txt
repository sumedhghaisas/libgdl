# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  a_move.cpp
  a_move.hpp
  argument.cpp
  argument.hpp
  a_state.cpp
  a_state.hpp
  b_move.cpp
  b_move.hpp
  b_state.cpp
  b_state.hpp
  clause.cpp
  clause.hpp
  error_type.hpp
  fact.cpp
  fact.hpp
  intrusive_wrapper.hpp
  location.hpp
  move_list.hpp
  move_vector.hpp
  str_var_map.hpp
  str_var_map.cpp
  variable_map.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
