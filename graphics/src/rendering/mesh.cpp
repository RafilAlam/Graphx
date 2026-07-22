#include <graphics/include/rendering/mesh.hpp>
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertexdata, std::vector<Face> facedata) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    m_vbo = buffers[0];
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexdata.size() * sizeof(Vertex), vertexdata.data(), GL_STATIC_DRAW);

    m_ebo = buffers[1];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, facedata.size() * sizeof(Face), facedata.data(), GL_STATIC_DRAW);
    m_indexcount = facedata.size() * 3;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, Position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, Normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, UV));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

GLuint Mesh::GetVAO() const {
    return m_vao;
}

GLuint Mesh::GetVBO() const {
    return m_vbo;
}

unsigned int Mesh::GetIndexCount() const {
    return m_indexcount;
}