# GDLParser test executable.
add_executable(libgdl_benchmark
  libgdl_benchmark.cpp
  cache_benchmarks.cpp
  gdlparser_benchmarks.cpp
  gdlreasoner_benchmarks.cpp
  unify_benchmarks.cpp
  flattener_benchmarks.cpp
  gdl_benchmarks.cpp
)
# Link dependencies of test executable.
target_link_libraries(libgdl_benchmark
  gdl
)
