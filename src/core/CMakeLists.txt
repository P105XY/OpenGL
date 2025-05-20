cmake_minimum_required(VERSION 3.16)
project(Trumpett LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cpp_renderer
    MODULE
    renderer.cpp
)

target_link_libraries(cpp_renderer
    PRIVATE
    renderer
)

set_target_properties(cpp_renderer PROPERTIES
    PREFIX ""
    OUTPUT_NAME "cpp_renderer"
    SUFFIX ".so"
)