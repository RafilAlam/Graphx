#pragma once

#include <dependencies/glad/glad.h>
#include <graphics/include/debug.hpp>
#include <graphics/include/utils.hpp>
#include <GLFW/glfw3.h>
#include <string>

class Shader {
public:
    Shader(std::string vertexsourcepath, std::string fragmentsourcepath);
    unsigned int GetProgram() const;
private:
    unsigned int m_program;
};