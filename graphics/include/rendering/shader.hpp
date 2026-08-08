#pragma once

#include <dependencies/glad/glad.h>
#include <graphics/include/debug.hpp>
#include <graphics/include/utils.hpp>
#include <GLFW/glfw3.h>
#include <string>

class Shader {
public:
    Shader(std::string vertexsourcepath, std::string fragmentsourcepath);
    Shader(const char* vertexsource, const char* fragmentsource);
    void Init(const char* vertexsource, const char* fragmentsource);
    unsigned int GetProgram() const;
private:
    unsigned int m_program;
};