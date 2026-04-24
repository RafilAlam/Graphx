#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <renderer/renderer.hpp>

#include <vector>

namespace graphx {

class Buffer {
public:
    Buffer(std::vector<float> Objects, int numObjects, size_t sizeofObject);
    ~Buffer();
    GLuint GetHandle();
    GLuint GetNumObjects();
private:
    GLuint handle;
    GLuint numobjects;
};

}