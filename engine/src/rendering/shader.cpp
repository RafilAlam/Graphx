#include <engine/include/rendering/shader.hpp>

Shader::Shader(const char* vertexsource, const char* fragmentsource) {
    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsource, NULL);
    glCompileShader(vertexshader);

    unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentsource, NULL);
    glCompileShader(fragmentshader);

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexshader);
    glAttachShader(m_program, fragmentshader);
    glLinkProgram(m_program);

    glUseProgram(m_program);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}