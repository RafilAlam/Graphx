#include <renderer/buffer.hpp>

namespace graphx {

    Buffer::Buffer(std::vector<float> Objects, int numObjects, size_t sizeofObject) : numobjects(numObjects) {
        glCreateBuffers(1, &handle);
        glNamedBufferStorage(handle, sizeofObject*numObjects, Objects.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    Buffer::~Buffer() {
        glDeleteBuffers(1, &handle);
    }

    GLuint Buffer::GetHandle() {
        return handle;
    }

    GLuint Buffer::GetNumObjects() {
        return numobjects;
    }

}