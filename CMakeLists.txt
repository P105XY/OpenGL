#전역 설정 CMake
cmake_minimum_required(VERSION 3.16)
project(Trumpett)

#어떤 C++버전 사용인지, 필요한지에 대한 설정을 지정.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_FINDPYTHON ON)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/dependencies/pybind11/include/pybind11")
find_package(pybind11 REQUIRED)

#별도로 필요한 include 경로를 잡아준다.
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include/glfw)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include/glad)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include/glm)
include_directories(${CMAKE_SOURCE_DIR}/src/core)

#메인으로 진행되는 경로를 잡아준다
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_subdirectory(src)
add_subdirectory(src/python)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/lib/libglfw.3.dylib"
)

add_library(glad STATIC IMPORTED)
set_target_properties(glad PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/include/glad/glad.h"
)

#프로그램이 실행될 때 컴파일되는 소스코드를 나열해서 잡아줌.
add_executable(Trumpett
src/core/main.cpp
src/core/glad.c
)

target_link_libraries(Trumpett glfw glad glm "-framework OpenGL")