include_directories(..) # <gdlparser/[whatever]>

# Add kif.hpp and data_types to list of sources.
set(GDLPARSER_SRCS ${GDLPARSER_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/kif.hpp")
set(GDLPARSER_SRCS ${GDLPARSER_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/data_types.hpp")
set(GDLPARSER_SRCS ${GDLPARSER_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/data_types.cpp")

## Recurse into both core/ and methods/.
set(DIRS
  parser
  util
  tests
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# GDLPARSER_SRCS is set in the subdirectories.
add_library(gdlparser SHARED ${GDLPARSER_SRCS})

set_target_properties(gdlparser
  PROPERTIES
  VERSION 1.0
)

# Make sure the linker can find the needed library.
# rt: clock_gettime()
target_link_libraries(gdlparser rt)

# Collect all header files in the library.
file(GLOB_RECURSE INCLUDE_H_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)
file(GLOB_RECURSE INCLUDE_HPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
set(INCLUDE_FILES ${INCLUDE_H_FILES} ${INCLUDE_HPP_FILES})

# Move all of these header files to <builddir>/include/gdlparser/ after the library
# is built.  First we have to create that directory though.
add_custom_command(TARGET gdlparser POST_BUILD
  COMMENT "Moving header files to include/gdlparser/"
  COMMAND ${CMAKE_COMMAND} ARGS -E
    make_directory ${CMAKE_BINARY_DIR}/include/gdlparser/)

# Then copy each of the header files over to that directory.
foreach(incl_file ${INCLUDE_FILES})
  add_custom_command(TARGET gdlparser POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${incl_file}
           ${CMAKE_BINARY_DIR}/include/gdlparser/${incl_file})
endforeach()

# At install time, we simply install that directory of header files we
# collected to include/.
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/gdlparser DESTINATION include)

# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
install(TARGETS gdlparser
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

# For 'make test'.
add_custom_target(test
  ${CMAKE_BINARY_DIR}/bin/gdlparser_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running gdlparser test"
  DEPENDS gdlparser_test)

add_executable(kif
  kif_main.cpp
)
target_link_libraries(kif
  gdlparser
  boost_program_options
)
install(TARGETS kif RUNTIME DESTINATION bin)
