# libGDL test executable.
add_executable(libgdl_test
  libgdl_test.cpp
  kif_tests.cpp
  gdlreasoner_tests.cpp
  argument_tests.cpp
  fact_tests.cpp
  clause_tests.cpp 
  cache_tests.cpp
  flattener_tests.cpp
  unify_tests.cpp
  b_state_tests.cpp
  gdl_tests.cpp
)
# Link dependencies of test executable.
target_link_libraries(libgdl_test
  gdl
  boost_unit_test_framework
)
