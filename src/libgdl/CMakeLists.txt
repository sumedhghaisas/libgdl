include_directories(..) # <libgdl/[whatever]>

# Add core.hpp to list of sources.
set(GDL_SRCS ${GDL_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/core.hpp")
set(GDL_SRCS ${GDL_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/gdl.hpp")
set(GDL_SRCS ${GDL_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/gdl_impl.hpp")
set(GDL_SRCS ${GDL_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/player_runner.hpp")
set(GDL_SRCS ${GDL_SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/player_runner_impl.hpp")

## Recurse into both gdlparser/ and gdlreasoner/.
set(DIRS
  connection
  core
  gdlparser
  player
  propnet
  reasoners
  state_machine
  tests
)

foreach(dir ${DIRS})
    add_subdirectory(${dir})
endforeach()

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "")

# don't add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# LIBGDL_SRCS is set in the subdirectories.
add_library(gdl SHARED ${GDL_SRCS})

add_dependencies(gdl copy_includes)

target_link_libraries(gdl
  ${Boost_LIBRARIES}
)

set_target_properties(gdl
  PROPERTIES
  VERSION ${LIBGDL_MAJOR_VERSION}
  SOVERSION ${LIBGDL_MINOR_VERSION}
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
  add_custom_command(TARGET libgdl_headers POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E
      copy ${CMAKE_CURRENT_SOURCE_DIR}/${incl_file}
           ${CMAKE_BINARY_DIR}/include/libgdl/${incl_file})
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

add_custom_target(libgdl_data)
# Copy test data into right place.
add_custom_command(TARGET libgdl_data POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../data/
      ${PROJECT_BINARY_DIR}/data/
)

# For 'make test'.
add_custom_target(test
  ${CMAKE_BINARY_DIR}/bin/libgdl_test
  COMMAND valgrind --leak-check=full --track-origins=yes ${CMAKE_BINARY_DIR}/bin/libgdl_test --run_test=*/*Test 3>&1 2>&3 >output | tee grindout
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running libGDL tests"
  DEPENDS libgdl_test)
  
add_dependencies(test libgdl_data)

# For 'make test'.
#add_custom_target(benchmark
#  ${CMAKE_BINARY_DIR}/bin/libgdl_benchmark
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
#  COMMENT "Running libGDL benchmarks"
#  DEPENDS libgdl_benchmark)
  
#add_dependencies(benchmark libgdl_data)

