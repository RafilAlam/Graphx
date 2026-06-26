#pragma once

#include <dependencies/glad/glad.h>

namespace graphx {

class Shader {
public:
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void Use() const;

private:
    GLuint program = 0;
    GLuint Compile(GLenum type, const char* source);
};

}
