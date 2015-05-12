cmake_minimum_required(VERSION 2.8.5)

project(libgdl C CXX)

# set version information
set(LIBGDL_MAJOR_VERSION "0")
set(LIBGDL_MINOR_VERSION "0")
set(LIBGDL_PATCH_VERSION "0")
set(LIBGDL_VERSION_NAME "master")

# First, define all the compilation options.
# We default to debugging mode for developers.
option(DEBUG "Compile with debugging information" OFF)
option(PROFILE "Compile with profiling information" OFF)
option(USE_STATIC_BOOST "Compile with linking boost statically" OFF)
option(USE_STATIC_LIBGDL "Compile executables with static libgdl" OFF)

# Set the CFLAGS and CXXFLAGS depending on the options the user specified.
# Only GCC-like compilers support -Wextra, and other compilers give tons of
# output for -Wall, so only -Wall and -Wextra on GCC.
if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

# Debugging CFLAGS.  Turn optimizations off; turn debugging symbols on.
if(DEBUG)
  add_definitions(-DDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
else()
  add_definitions(-DNDEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif(DEBUG)

# Profiling CFLAGS.  Turn profiling information on.
if(PROFILE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif(PROFILE)

# set path right
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

if(USE_STATIC_BOOST)
  set(Boost_USE_STATIC_LIBS   ON)
endif()

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

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# Make a target to generate the documentation.  If Doxygen isn't installed, then
# I guess this option will just be unavailable.
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # Preprocess the Doxyfile.  This is done before 'make doc'.
  add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Doxyfile
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -D DESTDIR="${CMAKE_BINARY_DIR}" -P
          "${CMAKE_CURRENT_SOURCE_DIR}/CMake/GenerateDoxyfile.cmake"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      COMMENT "Creating Doxyfile to generate Doxygen documentation"
  )

  # Generate documentation.
  add_custom_target(doc
      COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_BINARY_DIR}/Doxyfile"
      DEPENDS "${CMAKE_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      COMMENT "Generating API documentation with Doxygen"
  )

  install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html
      DESTINATION share/doc/libgdl
      COMPONENT doc
      OPTIONAL
  )
endif (DOXYGEN_FOUND)

# Recurse into the rest of the project.
add_subdirectory(src/libgdl)
