cmake_minimum_required(VERSION 3.25)

project(cds CXX)

# Tests, benches and global settings only when built on its own, never when a consumer adds it.
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 26)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Feeds .vscode/c_cpp_properties.json.
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    endif()

    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    endif()
endif()

find_package(Threads REQUIRED)

add_subdirectory(cmake/queue)
add_subdirectory(cmake/buffer)

# Every module.
add_library(cds INTERFACE)
add_library(cds::cds ALIAS cds)
target_link_libraries(cds INTERFACE cds::queue cds::buffer)

if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

include(CTest)                                  # BUILD_TESTING, default ON
option(CDS_BUILD_BENCHMARKS "Build Google Benchmark suites" ON)

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    # Every test_*.cpp under tests/. CONFIGURE_DEPENDS re-globs at build
    # time, so a new file needs no edit here.
    file(GLOB_RECURSE CDS_TEST_SRCS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/tests/test_*.cpp)
    add_executable(cds_tests ${CDS_TEST_SRCS})
    target_compile_options(cds_tests PRIVATE -Wall -Wextra)
    target_link_libraries(cds_tests PRIVATE cds::cds GTest::gtest GTest::gtest_main)
    add_test(NAME cds_tests COMMAND cds_tests)
endif()

if(CDS_BUILD_BENCHMARKS)
    include(FetchContent)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(googlebenchmark)

    # One binary, every benchmark case, one results table. CONFIGURE_DEPENDS
    # re-globs at build time, so a new file needs no edit here.
    file(GLOB_RECURSE CDS_BENCH_SRCS CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/benchmarks/bench_*.cpp
        ${PROJECT_SOURCE_DIR}/benchmarks/bench_main.cpp)
    add_executable(cds_bench ${CDS_BENCH_SRCS})
    target_compile_options(cds_bench PRIVATE -Wall -Wextra)
    target_include_directories(cds_bench PRIVATE ${PROJECT_SOURCE_DIR}/benchmarks/support)
    target_link_libraries(cds_bench PRIVATE cds::cds benchmark::benchmark)
    target_compile_definitions(cds_bench PRIVATE CDS_REPO_ROOT="${PROJECT_SOURCE_DIR}")
endif()
