#include <graphics/include/rendering/uniformbuffer.hpp>

UniformBuffer::UniformBuffer(size_t size, uint32_t binding) {
    glGenBuffers(1, &m_handle);

    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);

    glBufferData(
        GL_UNIFORM_BUFFER,
        size,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glBindBufferBase(
        GL_UNIFORM_BUFFER,
        binding,
        m_handle
    );
}