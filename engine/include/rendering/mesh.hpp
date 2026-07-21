#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <stddef.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

class Mesh {
public: 
    Mesh(std::vector<Vertex> vertexdata);
    unsigned int GetVAO() const;
private:
    unsigned int m_vbo;
    unsigned int m_vao;
};