#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

class UniformBuffer {
public:
    UniformBuffer(size_t size, uint32_t binding);
    template<typename T>
    void Upload(const T& data) {
        glBindBuffer(GL_UNIFORM_BUFFER, m_handle);

        glBufferSubData(
            GL_UNIFORM_BUFFER,
            0,
            sizeof(T),
            &data
        );
    }
private:
    GLuint m_handle;
};