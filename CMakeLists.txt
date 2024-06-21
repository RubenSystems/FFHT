cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(benchmark REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(main main.cpp)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(main PRIVATE -g)

add_executable(perf_main main.cpp)
set_target_properties(perf_main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(perf_main PRIVATE -O3 -march=native -flto)

add_executable(bench benchmarks/benchmark.cpp)
set_target_properties(bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(bench PRIVATE -O3 -march=native -flto)
target_link_libraries(bench benchmark::benchmark)




add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/main
    DEPENDS main
    COMMENT "Running main"
)
add_custom_target(prun
    COMMAND ${CMAKE_BINARY_DIR}/bin/perf_main
    DEPENDS perf_main
    COMMENT "Running perf_main"
)

add_custom_target(benchmark
    COMMAND ${CMAKE_BINARY_DIR}/bin/bench --benchmark_out=bench_out.txt --benchmark_out_format=console
    DEPENDS bench
    COMMENT "Running perf_main"
)
