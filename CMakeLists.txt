#전역 설정 CMake
cmake_minimum_required(VERSION 3.14)
project(OpenGLRenderer)

#외부 라이브러리 경로 (glfw, glm 등)
include_directories(dependencies/include)
include_directories(dependencies/lib)
include_directories(dependencies/pybind11)

#서브 프로젝트 (opengl rederer, py)
add_subdirectory(src/core)
add_subdirectory(binding)