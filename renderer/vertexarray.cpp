#include <renderer/vertexarray.hpp>

namespace graphx {

VertexArray::VertexArray() = default;

VertexArray::VertexArray(GLuint VBO, GLuint EBO) {
    glCreateVertexArrays(1, &handle);
    glVertexArrayVertexBuffer(handle, 0, VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(handle, EBO);

    glEnableVertexArrayAttrib(handle, 0);
    glVertexArrayAttribFormat(handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribBinding(handle, 0, 0);
}

VertexArray::VertexArray(VertexArray&& other) noexcept : handle(other.handle) {
    other.handle = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    if (this != &other) {
        if (handle != 0) {
            glDeleteVertexArrays(1, &handle);
        }
        handle = other.handle;
        other.handle = 0;
    }
    return *this;
}

VertexArray::~VertexArray() {
    if (handle != 0) {
        glDeleteVertexArrays(1, &handle);
    }
}

GLuint VertexArray::GetHandle() const {
    return handle;
}

}
