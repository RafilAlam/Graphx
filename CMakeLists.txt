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

add_subdirectory(extern/yyjson)
add_subdirectory(extern/stb_image)
add_subdirectory(extern/uuid_v4)
add_subdirectory(extern/glad)

add_library(Graphx STATIC
 engine/core/src/app.cpp engine/core/src/window.cpp engine/core/src/debug.cpp engine/core/src/utils.cpp engine/graphics/src/stb_image.cpp
 engine/graphics/src/project/project.cpp engine/graphics/src/project/entity.cpp
 engine/graphics/src/rendering/renderer.cpp engine/graphics/src/rendering/debugrenderer.cpp engine/graphics/src/rendering/uniformbuffer.cpp engine/graphics/src/rendering/assetmanager.cpp engine/graphics/src/rendering/material.cpp engine/graphics/src/rendering/texture.cpp engine/graphics/src/rendering/shader.cpp engine/graphics/src/rendering/mesh.cpp
 
 engine/physics/src/physicsworld.cpp engine/physics/src/integrator.cpp engine/physics/src/rigidbody.cpp engine/physics/src/collisionsolver.cpp engine/physics/src/joint.cpp
)
target_include_directories(Graphx PRIVATE .)
target_link_libraries(Graphx PUBLIC OpenGL::GL stb_image glfw glad glm::glm assimp uuid_v4::uuid_v4 yyjson)

add_executable(TransformDemo sandbox/transformdemo.cpp)
target_include_directories(TransformDemo PRIVATE .)
target_link_options(TransformDemo PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)
target_link_libraries(TransformDemo PRIVATE Graphx)

add_executable(Editor editor/src/editor.cpp)
target_include_directories(Editor PRIVATE .)
target_link_options(Editor PRIVATE
    -static
    -static-libgcc
    -static-libstdc++
)
target_link_libraries(Editor PRIVATE Graphx)