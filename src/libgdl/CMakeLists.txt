include_directories(..) # <mlpack/[whatever]>

# Add core.hpp to list of sources.
set(MLPACK_SRCS ${MLPACK_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/core.hpp")

## Recurse into both gdlparser/ and gdlreasoner/.
set(DIRS
  gdlparser
  tests
  gdlreasoner
  core
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# LIBGDL_SRCS is set in the subdirectories.
add_library(gdl SHARED ${GDL_SRCS})

add_dependencies(gdl copy_includes)

target_link_libraries(gdl
  ${Boost_LIBRARIES}
)

set_target_properties(gdl
  PROPERTIES
  VERSION 1.0
)

# Make sure the linker can find the needed library.
# rt: clock_gettime()
target_link_libraries(gdl rt)

# Collect all header files in the library.
file(GLOB_RECURSE INCLUDE_H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE INCLUDE_HPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE INCLUDE_HH_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hh)
set(INCLUDE_FILES ${INCLUDE_H_FILES} ${INCLUDE_HPP_FILES} ${INCLUDE_HH_FILES})

# Move all of these header files to <builddir>/include/libgdl/ after the library
# is built.  First we have to create that directory though.
add_custom_target(libgdl_headers)
add_custom_command(TARGET libgdl_headers POST_BUILD
  COMMENT "Moving header files to include/libgdl/"
  COMMAND ${CMAKE_COMMAND} ARGS -E
    make_directory ${CMAKE_BINARY_DIR}/include/libgdl/)
    
# Then copy each of the header files over to that directory.
foreach(incl_file ${INCLUDE_FILES})
  add_custom_command(TARGET mlpack_headers POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${incl_file}
           ${CMAKE_BINARY_DIR}/include/mlpack/${incl_file})
endforeach()

# At install time, we simply install that directory of header files we
# collected to include/.
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/libgdl DESTINATION include)

# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
install(TARGETS gdl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
  
add_dependencies(gdl libgdl_headers)

# For 'make test'.
add_custom_target(test
  ${CMAKE_BINARY_DIR}/bin/libgdl_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running libgdl tests"
  DEPENDS libgdl_test)
