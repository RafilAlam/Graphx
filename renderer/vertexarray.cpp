#include <renderer/vertexarray.hpp>

namespace graphx {

    VertexArray::VertexArray(GLuint VBO, GLuint EBO) {
        glCreateVertexArrays(1, &handle);
        glVertexArrayVertexBuffer(handle, 0, VBO, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(handle, EBO);

        glEnableVertexArrayAttrib(handle, 0);
        glVertexArrayAttribFormat(handle, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribBinding(handle, 0, 0);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &handle);
    }

    GLuint Buffer::GetHandle() {
        return handle;
    }

}