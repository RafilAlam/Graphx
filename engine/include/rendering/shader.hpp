#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
public:
    Shader(const char* vertexsource, const char* fragmentsource);
private:
    unsigned int m_program;
};