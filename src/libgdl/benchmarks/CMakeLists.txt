# GDLParser test executable.
add_executable(libgdl_benchmark
  libgdl_benchmark.cpp
  cache_benchmarks.cpp
)
# Link dependencies of test executable.
target_link_libraries(libgdl_benchmark
  gdl
)
