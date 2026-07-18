#pragma once

#include <dependencies/glad/glad.h>
#include <engine/include/debug.hpp>
#include <GLFW/glfw3.h>

struct ShaderConfig {
    const char* vertexsource = 
    R"(#version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    })";
    const char* fragmentsource =
    R"(#version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    })";
};

class Shader {
public:
    Shader(ShaderConfig config);
    unsigned int GetProgram() const;
private:
    unsigned int m_program;
};