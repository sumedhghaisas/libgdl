# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  thread_pool.hpp
  thread_pool_impl.hpp
  thread_pool.cpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)
