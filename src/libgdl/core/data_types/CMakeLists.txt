# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  argument.hpp
  argument.cpp
  fact.hpp
  fact.cpp
  clause.hpp
  clause.cpp
  location.hpp
  b_state.hpp
  b_state.cpp
  b_move.hpp
  b_move.cpp
  move_list.hpp
  move_vector.hpp
  intrusive_wrapper.hpp
  str_var_map.hpp
  str_var_map.cpp
  error_type.hpp
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
