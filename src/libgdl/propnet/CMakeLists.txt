## Recurse parser
set(DIRS
  node_types
  entry_types
  handlers
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  entry_manager.cpp
  entry_manager.hpp
  imove.cpp
  imove.hpp
  istate.cpp
  istate.hpp
  memory_manager.hpp
  node_types.hpp
  propnet.cpp
  propnet.hpp
  propnet_impl.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
