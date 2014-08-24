# GDLParser test executable.
add_executable(libgdl_test
  libgdl_test.cpp
  argument_tests.cpp
  cache_tests.cpp
  kif_tests.cpp
  knowledgebase_tests.cpp
  state_tests.cpp
  gdl_tests.cpp
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
