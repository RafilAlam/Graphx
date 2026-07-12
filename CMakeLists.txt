cmake_minimum_required(VERSION 4.1.0)
project(Graphx LANGUAGES C CXX)
include(FetchContent)

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

add_library(Graphx STATIC engine/src/debug.cpp)
target_include_directories(Graphx PRIVATE .)
target_link_libraries(Graphx PUBLIC glfw glad glm::glm)

add_executable(App sandbox/main.cpp)
target_include_directories(App PRIVATE .)
target_link_libraries(App PRIVATE Graphx)