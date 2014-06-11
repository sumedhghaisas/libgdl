cmake_minimum_required(VERSION 2.8.5)
project(gdlparser C CXX)

# First, define all the compilation options.
# We default to debugging mode for developers.
option(DEBUG "Compile with debugging information" ON)

# Set the CFLAGS and CXXFLAGS depending on the options the user specified.
# Only GCC-like compilers support -Wextra, and other compilers give tons of
# output for -Wall, so only -Wall and -Wextra on GCC.
if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCC)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Debugging CFLAGS.  Turn optimizations off; turn debugging symbols on.
if(DEBUG)
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -O0")
else()
  add_definitions(-DNDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3")
endif(DEBUG)

find_package(Boost
    COMPONENTS
      program_options
      unit_test_framework
    REQUIRED
)
include_directories(${Boost_INCLUDE_DIRS})

# Save the actual link paths (because they will get overwritten if we discover
# we need to find Boost.Random too).
set(Boost_BACKUP_LIBRARIES ${Boost_LIBRARIES})

link_directories(${Boost_LIBRARY_DIRS})

# For Boost testing framework (will have no effect on non-testing executables).
# This specifies to Boost that we are dynamically linking to the Boost test
# library.
add_definitions(-DBOOST_TEST_DYN_LINK)

# Create a 'distclean' target in case the user is using an in-source build for
# some reason.
include(CMake/TargetDistclean.cmake OPTIONAL)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

# Recurse into the rest of the project.
add_subdirectory(src/gdlparser)
