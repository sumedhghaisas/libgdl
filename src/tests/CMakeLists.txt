# GDLParser test executable.
add_executable(libgdl_test
  gdlparser_test.cpp
  kif_test.cpp
  defined_rel_test.cpp
  rule_safety_test.cpp
  stratified_test.cpp
  recursive_depen_test.cpp
)
# Link dependencies of test executable.
target_link_libraries(libgdl_test
  gdl
  boost_unit_test_framework
)

# Copy test data into right place.
add_custom_command(TARGET libgdl_test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/
      ${PROJECT_BINARY_DIR}/data/
)
