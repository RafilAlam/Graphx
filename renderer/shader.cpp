#include <renderer/shader.hpp>
#include <utilities/utils.hpp>

namespace graphx {

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    GLuint vertex = Compile(GL_VERTEX_SHADER, vertexSource);
    GLuint fragment = Compile(GL_FRAGMENT_SHADER, fragmentSource);

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        Error(std::string("Shader link failed: ") + log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

void Shader::Use() const {
    glUseProgram(program);
}

GLuint Shader::Compile(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        Error(std::string("Shader compile failed: ") + log);
    }

    return shader;
}

}
