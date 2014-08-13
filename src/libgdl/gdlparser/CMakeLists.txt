## Recurse parser
set(DIRS
  parser
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  kif.hpp
  kif.cpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)

add_executable(kif
  kif_main.cpp
)

target_link_libraries(kif
  gdl
  boost_program_options
)

set_target_properties(kif PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

install(TARGETS kif RUNTIME DESTINATION bin)
