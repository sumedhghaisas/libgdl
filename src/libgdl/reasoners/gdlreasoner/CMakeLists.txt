## Recurse into both core/ and methods/.
set(DIRS
  logicbase
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# Define the files we need to compile.
# Anything not in this list will not be compiled into libGDL.
set(SOURCES
  gdlreasoner.hpp
  gdlreasoner_impl.hpp
  gdlreasoner.cpp
  knowledgebase.cpp
  knowledgebase.hpp
  kif_flattener.cpp
  kif_flattener.hpp
)

# add directory name to sources
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all libgdl sources (used at
# the parent scope).
set(GDL_SRCS ${GDL_SRCS} ${DIR_SRCS} PARENT_SCOPE)

#add_executable(flatten
#  kif_flattener_main.cpp
#)

#target_link_libraries(flatten
#  gdl
#  boost_program_options
#  pthread
#)

#set_target_properties(flatten PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

#install(TARGETS flatten RUNTIME DESTINATION bin)
