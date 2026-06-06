cmake_minimum_required(VERSION 3.20)

project(
    tiny-inference
    VERSION 1.0
    LANGUAGES CXX
)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(tensor
    src/tensor.cpp
    src/GEMM.cpp
    src/linear.cpp
    src/relu.cpp
    src/sequence.cpp
    src/utils.cpp
)
target_compile_options(tensor PRIVATE
    -march=native
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
add_executable(test_gemm_tiled
    test/gemm_test3.cpp
)
add_executable(test_linear_1
    test/linear_1.cpp
)
add_executable(test_relu_1
    test/relu1.cpp
)
add_executable(test_linear_relu_1
    test/linear_relu1.cpp
)
add_executable(test_linear_relu_layer1
    test/linear_relu_layer1.cpp
)
add_executable(test_sequence
    test/sequence1.cpp
)
add_executable(test_sequence2
    test/sequence2.cpp
)
add_executable(test_loader_1
    test/loadertest1.cpp
)
add_executable(test_loader_2
    test/loadertest2.cpp
)
target_link_libraries(test_loader_2 PRIVATE tensor)
target_link_libraries(test_loader_1 PRIVATE tensor)
target_link_libraries(test_sequence2 PRIVATE tensor)
target_link_libraries(test_sequence PRIVATE tensor)
target_link_libraries(test_linear_relu_layer1 PRIVATE tensor)
target_link_libraries(test_linear_relu_1 PRIVATE tensor)
target_link_libraries(test_relu_1 PRIVATE tensor)
target_link_libraries(test_linear_1 PRIVATE tensor)
target_link_libraries(test_gemm_tiled PRIVATE tensor)
target_link_libraries(test_gemm_ijk PRIVATE tensor)
target_link_libraries(benchmark_gemm1 PRIVATE tensor)
target_link_libraries(test_tensor PRIVATE tensor)
target_link_libraries(test_gemm PRIVATE tensor)
set(TEST_TARGETS
    test_tensor test_gemm test_gemm_ijk test_gemm_tiled
    test_linear_1 test_relu_1 test_linear_relu_1
    test_linear_relu_layer1 test_sequence test_sequence2
    test_loader_1 test_loader_2
)
target_compile_options(tensor PRIVATE
    -march=native
    -UNDEBUG
)

foreach(t IN LISTS TEST_TARGETS)
    target_compile_options(${t} PRIVATE -UNDEBUG)
endforeach()
