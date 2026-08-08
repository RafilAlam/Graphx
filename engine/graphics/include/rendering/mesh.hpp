#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <stddef.h>

struct Face {
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

class Mesh {
public: 
    Mesh(std::vector<Vertex> vertexdata, std::vector<Face> facedata);
    GLuint GetVAO() const;
    GLuint GetVBO() const;
    unsigned int GetIndexCount() const;
private:
    unsigned int m_indexcount;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_vao;
};