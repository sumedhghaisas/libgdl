# Define the files we need to compile.
# Anything not in this list will not be compiled into GDLParser.
set(SOURCES
  setop.cpp
  setop.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all Epicurus sources (used at
# the parent scope).
set(GDLPARSER_SRCS ${GDLPARSER_SRCS} ${DIR_SRCS} PARENT_SCOPE)
