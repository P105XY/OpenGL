cmake_minimum_required(VERSION 3.16)
project(Trumpett LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(pybind11 REQUIRED)

add_subdirectory(src/core)

add_library(renderer
    src/core/glad.c
    src/core/private/Camera.cpp
    src/core/private/Light.cpp
    src/core/private/Texture.cpp
    src/core/private/Shader.cpp
    src/core/private/FileSupport.cpp # FileFromString 구현 포함
)

# include 경로
target_include_directories(renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_SOURCE_DIR}/dependencies/include/glfw/3.4/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/public
)

# 실제로 필요한 라이브러리만 링크
target_link_libraries(renderer PUBLIC
    "${CMAKE_SOURCE_DIR}/dependencies/lib/libglfw.3.4.dylib" # e.g. -lglfw
      "-framework Cocoa"
  "-framework OpenGL"
  "-framework IOKit"
  "-framework CoreVideo"
)

# ------------------------------------------------------------------------------
# 실행 바이너리: main.cpp
# ------------------------------------------------------------------------------
add_executable(app src/core/main.cpp)
target_link_libraries(app PRIVATE renderer)
