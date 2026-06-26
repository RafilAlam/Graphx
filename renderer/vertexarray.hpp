#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <renderer/vertex.hpp>

#include <cstddef>

namespace graphx {

class VertexArray {
public:
    VertexArray();
    VertexArray(GLuint VBO, GLuint EBO);
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    GLuint GetHandle() const;
private:
    GLuint handle = 0;
};

}
