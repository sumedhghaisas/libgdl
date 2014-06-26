include_directories(gdlparser/..) 
include_directories(gdlreasoner/..)

## Recurse into both gdlparser/ and gdlreasoner/.
set(DIRS
  gdlparser
  tests
  gdlreasoner
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

set(INCLUDE_FILES)
foreach(FILE ${GDL_SRCS})
  #message("${FILE}")
  if(FILE MATCHES "(.*)([.]hpp|[.]hh|[.]h)")
        STRING(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" REL_FILE ${FILE})
	list(APPEND INCLUDE_FILES ${REL_FILE})
  endif()
endforeach()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/)

add_custom_target(process_includes ALL COMMAND ${CMAKE_COMMAND} -E tar cfj ${CMAKE_BINARY_DIR}/include/temp.tar ${INCLUDE_FILES}
					COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/include/gdlparser
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Processing all the header files...")

add_custom_target(copy_includes ALL COMMAND ${CMAKE_COMMAND} -E tar xfj temp.tar ${FileList}
			   COMMAND ${CMAKE_COMMAND} -E remove temp.tar
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/include/
    COMMENT "Copying header files to include...")

add_dependencies(copy_includes process_includes)

# At install time, we simply install that directory of header files we
# collected to include/.
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION include)

# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
# Set generated executables to be installed.  Unfortunately they must manually
# be entered...
install(TARGETS gdl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

# For 'make test'.
add_custom_target(test
  ${CMAKE_BINARY_DIR}/bin/libgdl_test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ # This is where test files are put.
  COMMENT "Running libgdl tests"
  DEPENDS libgdl_test)
