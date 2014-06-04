# GDLParser test executable.
add_executable(gdlparser_test
  gdlparser_test.cpp
  kif_test.cpp
)
# Link dependencies of test executable.
target_link_libraries(gdlparser_test
  gdlparser
  boost_unit_test_framework
)

# Copy test data into right place.
add_custom_command(TARGET gdlparser_test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/
      ${PROJECT_BINARY_DIR}/data/
)
