# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  and_node.cpp
  and_node.hpp
  base_node.cpp
  base_node.hpp
  goal_node.cpp
  goal_node.hpp
  input_node.cpp
  input_node.hpp
  legal_node.cpp
  legal_node.hpp
  next_node.cpp
  next_node.hpp
  node.hpp
  not_node.cpp
  not_node.hpp
  or_node.cpp
  or_node.hpp
  terminal_node.cpp
  terminal_node.hpp
  view_node.cpp
  view_node.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
