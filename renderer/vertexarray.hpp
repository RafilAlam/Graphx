#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <renderer/renderer.hpp>

#include <cstddef>

namespace graphx {

class VertexArray {
public:
    VertexArray(GLuint VBO, GLuint EBO);
    ~VertexArray();
    GLuint GetHandle();
private:
    GLuint handle;
};

}