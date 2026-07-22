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

fetchcontent_declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v6.0.5
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

fetchcontent_makeavailable(assimp)

fetchcontent_declare(
  ryml
  GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
  GIT_TAG v0.15.2
)

set(RYML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RYML_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(RYML_BUILD_API OFF CACHE BOOL "" FORCE)
set(RYML_INSTALL OFF CACHE BOOL "" FORCE)

fetchcontent_makeavailable(ryml)

add_library(glad STATIC dependencies/glad/glad.c)
target_include_directories(glad PRIVATE .)

add_library(Graphx STATIC
 graphics/src/app.cpp graphics/src/window.cpp graphics/src/debug.cpp graphics/src/utils.cpp graphics/src/stb_image.cpp
 graphics/src/scene/scene.cpp graphics/src/scene/object.cpp
 graphics/src/rendering/renderer.cpp graphics/src/rendering/uniformbuffer.cpp graphics/src/rendering/assetmanager.cpp graphics/src/rendering/material.cpp graphics/src/rendering/texture.cpp graphics/src/rendering/shader.cpp graphics/src/rendering/mesh.cpp
 
 physics/src/integrator.cpp physics/src/rigidbody.cpp
)
target_include_directories(Graphx PRIVATE .)
target_link_libraries(Graphx PUBLIC OpenGL::GL glfw glad glm::glm assimp ryml::ryml)

add_executable(InputDemo sandbox/inputdemo.cpp)
target_include_directories(InputDemo PRIVATE .)
target_link_libraries(InputDemo PRIVATE Graphx)

add_executable(TransformDemo sandbox/transformdemo.cpp)
target_include_directories(TransformDemo PRIVATE .)
target_link_options(TransformDemo PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)
target_link_libraries(TransformDemo PRIVATE Graphx)