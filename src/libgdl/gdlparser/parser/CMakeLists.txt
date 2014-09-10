## Recurse parser
set(DIRS
 syntax_tree_types
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  kif_driver.cpp
  kif_driver.hpp
  kif_parser.tab.cc
  kif_parser.tab.hh
  FlexLexer.h
  lex.yy.cc
  kif_scanner.hpp
  location.hh
  position.hh
  stack.hh
  syntax_tree_types_decl.hpp
  syntax_tree_types_includes.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
