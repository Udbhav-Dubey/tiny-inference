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

target_link_libraries(test_tensor PRIVATE tensor)
