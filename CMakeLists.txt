cmake_minimum_required(VERSION 3.20)

project(
    tiny-inference
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(tensor
    src/tensor.cpp
    src/GEMM.cpp
)

target_include_directories(tensor PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(tiny-inference
    src/main.cpp
)

target_link_libraries(tiny-inference PRIVATE tensor)

add_executable(test_tensor
    test/tensor_test1.cpp
)
add_executable(test_gemm
    test/gemm_test1.cpp
)
add_executable(benchmark_gemm1
    benchmarking/gemm_benchmark_1.cpp
)
add_executable(test_gemm_ijk
    test/gemm_test2.cpp
)
target_link_libraries(test_gemm_ijk PRIVATE tensor)
target_link_libraries(benchmark_gemm1 PRIVATE tensor)
target_link_libraries(test_tensor PRIVATE tensor)
target_link_libraries(test_gemm PRIVATE tensor)
