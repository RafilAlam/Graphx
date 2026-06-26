cmake_minimum_required(VERSION 4.1.0)
project(Graphx LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

fetchcontent_declare(
  flecs
  GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
  GIT_TAG v4.0.4
  GIT_SHALLOW ON
)

fetchcontent_makeavailable(flecs)

add_library(glad STATIC dependencies/glad/glad.c)
target_include_directories(glad PUBLIC . dependencies)

add_library(Graphx STATIC
  core/window.cpp
  application/app.cpp
  assetmanager/assetmanager.cpp
  scene/mesh_factory.cpp
  renderer/renderer.cpp
  renderer/buffer.cpp
  renderer/vertexarray.cpp
  renderer/shader.cpp
  utilities/utils.cpp
)
target_include_directories(Graphx PRIVATE .)
target_link_libraries(Graphx PUBLIC glfw glad glm::glm flecs::flecs_static)

if(WIN32 AND NOT MSVC)
  target_link_libraries(Graphx PUBLIC ws2_32 dbghelp)
endif()

add_executable(App workspace/main.cpp)
target_include_directories(App PRIVATE .)
target_link_libraries(App PRIVATE Graphx)
