#전역 설정 CMake
cmake_minimum_required(VERSION 3.16)
project(Trumpett LANGUAGES C CXX)

#어떤 C++버전 사용인지, 필요한지에 대한 설정을 지정.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#별도로 필요한 include 경로를 잡아준다.
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)

#메인으로 진행되는 경로를 잡아준다.
add_subdirectory(src)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_subdirectory(src/python)

#프로그램이 실행될 때 컴파일되는 소스코드를 나열해서 잡아줌.
add_executable(Trumpett
src/core/main.cpp
src/core/glad.c
)