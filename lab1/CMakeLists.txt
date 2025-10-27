cmake_minimum_required(VERSION 3.15)
project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

include(FetchContent)
# ---------------------------------------------
# Fetch Google Benchmark
# ---------------------------------------------
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4
)
FetchContent_MakeAvailable(benchmark)

# ---------------------------------------------
# Fetch FlameGraph
# ---------------------------------------------
set(FLAMEGRAPH_DIR "${CMAKE_BINARY_DIR}/FlameGraph")
if(NOT EXISTS ${FLAMEGRAPH_DIR})
  file(DOWNLOAD
    https://github.com/brendangregg/FlameGraph/archive/refs/heads/master.zip
    ${CMAKE_BINARY_DIR}/flamegraph.zip
    SHOW_PROGRESS
  )
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/flamegraph.zip
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  file(RENAME ${CMAKE_BINARY_DIR}/FlameGraph-master ${FLAMEGRAPH_DIR})
endif()

# ---------------------------------------------
# Add own files
# ---------------------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*/*.cpp")
add_executable(lab1 ${SOURCES})
target_include_directories(lab1 PRIVATE ${CMAKE_SOURCE_DIR}/src/own_implementation)
target_include_directories(lab1 PRIVATE ${CMAKE_SOURCE_DIR}/src/test_ops)

# ---------------------------------------------
# Link external libraries
# ---------------------------------------------
find_package(Threads REQUIRED)
target_link_libraries(lab1 PRIVATE benchmark::benchmark Threads::Threads)

# ---------------------------------------------
# Add flags for optimization, profilling and code reduction
# ---------------------------------------------
target_compile_options(lab1 PRIVATE -O2 -g -fno-omit-frame-pointer)

# ---------------------------------------------
# Set properties
# ---------------------------------------------
set_target_properties(lab1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
