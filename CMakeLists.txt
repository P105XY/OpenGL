cmake_minimum_required(VERSION 3.16)
project(Trumpett LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)       # OpenGL::GL
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

# ------------------------------------------------------------------------------
#  renderer 라이브러리: glad, 셰이더, 카메라, 라이트, 파일지원 등 구현 모듈
# ------------------------------------------------------------------------------
add_library(renderer
  src/core/glad.c
  src/core/private/Camera.cpp
  src/core/private/Light.cpp
  src/core/private/Texture.cpp
  src/core/private/Shader.cpp
  src/core/private/FileSupport.cpp           # FileFromString 구현 포함
)

# include/search 경로
target_include_directories(renderer PUBLIC
  ${GLFW3_INCLUDE_DIRS}             # GLFW 헤더
  ${CMAKE_SOURCE_DIR}/dependencies/include  # glad, glfw, glm 헤더
  ${CMAKE_SOURCE_DIR}/src/core
  ${CMAKE_SOURCE_DIR}/src/core/public
)

# 링크 디렉토리·라이브러리
target_link_directories(renderer PUBLIC
  ${GLFW3_LIBRARY_DIRS}
)
target_link_libraries(renderer PUBLIC
  ${GLFW3_LIBRARIES}  # -lglfw
  OpenGL::GL          # -framework OpenGL
)

# 헤더 온리 GLM
target_include_directories(renderer PUBLIC
  ${CMAKE_SOURCE_DIR}/dependencies/include/glm
)

# ------------------------------------------------------------------------------
#  실행 바이너리: main.cpp
# ------------------------------------------------------------------------------
add_executable(app src/core/main.cpp)
target_link_libraries(app PRIVATE renderer)