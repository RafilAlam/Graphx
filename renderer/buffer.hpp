#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace graphx {

class Buffer {
public:
    Buffer();
    Buffer(std::vector<unsigned int> Objects, int numObjects, size_t sizeofObject);
    Buffer(std::vector<float> Objects, int numObjects, size_t sizeofObject);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept;
    Buffer& operator=(Buffer&& other) noexcept;

    GLuint GetHandle() const;
    GLuint GetNumObjects() const;
private:
    GLuint handle = 0;
    GLuint numobjects = 0;
};

}
