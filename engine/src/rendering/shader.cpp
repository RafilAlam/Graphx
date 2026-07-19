#include <engine/include/rendering/shader.hpp>

Shader::Shader(std::string vertexsourcepath, std::string fragmentsourcepath) {
    DebugPrint("Loading Shaders...");

    std::string vertexsource = LoadTextFile(vertexsourcepath);
    std::string fragmentsource = LoadTextFile(fragmentsourcepath);

    const char* charvertexsource = vertexsource.data();
    const char* charfragmentsource = fragmentsource.data();

    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &charvertexsource, NULL);
    glCompileShader(vertexshader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &charfragmentsource, NULL);
    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexshader);
    glAttachShader(m_program, fragmentshader);
    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
    DebugPrint("Loaded Shaders!");

    glUseProgram(m_program);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}

unsigned int Shader::GetProgram() const {
    return m_program;
}