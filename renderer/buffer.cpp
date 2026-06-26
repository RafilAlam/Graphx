#include <renderer/buffer.hpp>

namespace graphx {

Buffer::Buffer() = default;

Buffer::Buffer(std::vector<unsigned int> Objects, int numObjects, size_t sizeofObject) : numobjects(numObjects) {
    glCreateBuffers(1, &handle);
    glNamedBufferStorage(handle, sizeofObject * numObjects, Objects.data(), GL_DYNAMIC_STORAGE_BIT);
}

Buffer::Buffer(std::vector<float> Objects, int numObjects, size_t sizeofObject) : numobjects(numObjects) {
    glCreateBuffers(1, &handle);
    glNamedBufferStorage(handle, sizeofObject * numObjects, Objects.data(), GL_DYNAMIC_STORAGE_BIT);
}

Buffer::Buffer(Buffer&& other) noexcept : handle(other.handle), numobjects(other.numobjects) {
    other.handle = 0;
    other.numobjects = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        if (handle != 0) {
            glDeleteBuffers(1, &handle);
        }
        handle = other.handle;
        numobjects = other.numobjects;
        other.handle = 0;
        other.numobjects = 0;
    }
    return *this;
}

Buffer::~Buffer() {
    if (handle != 0) {
        glDeleteBuffers(1, &handle);
    }
}

GLuint Buffer::GetHandle() const {
    return handle;
}

GLuint Buffer::GetNumObjects() const {
    return numobjects;
}

}
