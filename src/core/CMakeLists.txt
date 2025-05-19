cmake_minimum_required(VERSION 3.16)
project(Trumpett LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#cpp_renderer는 파이썬 연결용으로만 사용
add_library(cpp_renderer
    MODULE
    renderer.cpp
)

target_link_libraries(cpp_renderer
    PRIVATE
    pybind11::module
    renderer
)

set_target_properties(cpp_renderer PROPERTIES
    PREFIX ""
    OUTPUT_NAME "cpp_renderer"
    SUFFIX ".so"
)