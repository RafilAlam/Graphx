#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Mesh {
public: 
    Mesh(std::vector<float> vertices);
    unsigned int GetVAO() const;
private:
    unsigned int m_vbo;
    unsigned int m_vao;
};