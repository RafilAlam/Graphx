#include <engine/graphics/include/rendering/shader.hpp>

Shader::Shader(std::string vertexsourcepath, std::string fragmentsourcepath) {
    std::string vertexsource = LoadTextFile(vertexsourcepath);
    const char* charvertexsource = vertexsource.data();

    if (fragmentsourcepath.length() > 0) {
        std::string fragmentsource = LoadTextFile(fragmentsourcepath);
        const char* charfragmentsource = fragmentsource.data();
        Init(charvertexsource, charfragmentsource);
    }

    Init(charvertexsource);
}

Shader::Shader(const char* vertexsource, const char* fragmentsource) {
    Init(vertexsource, fragmentsource);
}

void Shader::Init(const char* vertexsource, const char* fragmentsource) {
    DebugPrint("Loading Shaders...");

    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsource, NULL);
    glCompileShader(vertexshader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentsource, NULL);
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

void Shader::Init(const char* vertexsource) {
    DebugPrint("Loading Shaders...");

    unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexsource, NULL);
    glCompileShader(vertexshader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexshader);
    glLinkProgram(m_program);

    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        DebugError("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
    }
    DebugPrint("Loaded Shaders!");

    glUseProgram(m_program);
    glDeleteShader(vertexshader);
}

unsigned int Shader::GetProgram() const {
    return m_program;
}