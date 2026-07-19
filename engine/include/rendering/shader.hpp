#pragma once

#include <dependencies/glad/glad.h>
#include <engine/include/debug.hpp>
#include <engine/include/utils.hpp>
#include <GLFW/glfw3.h>
#include <string>

class Shader {
public:
    Shader(std::string vertexsourcepath, std::string fragmentsourcepath);
    unsigned int GetProgram() const;
private:
    unsigned int m_program;
};