cmake_minimum_required(VERSION 4.1.0)
project(Graphx LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(FetchContent)

find_package(OpenGL REQUIRED)

fetchcontent_declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.4
  GIT_SHALLOW ON
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

fetchcontent_makeavailable(glfw)

fetchcontent_declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.3
)

fetchcontent_makeavailable(glm)

add_library(glad STATIC dependencies/glad/glad.c)
target_include_directories(glad PRIVATE .)

add_library(Graphx STATIC engine/src/app.cpp engine/src/window.cpp engine/src/debug.cpp engine/src/scene.cpp engine/src/rendering/renderer.cpp engine/src/rendering/shader.cpp engine/src/rendering/mesh.cpp)
target_include_directories(Graphx PRIVATE .)
target_link_libraries(Graphx PUBLIC OpenGL::GL glfw glad glm::glm)

add_executable(App sandbox/main.cpp)
target_include_directories(App PRIVATE .)
target_link_libraries(App PRIVATE Graphx)