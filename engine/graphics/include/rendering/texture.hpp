#pragma once

#include <dependencies/glad/glad.h>
#include <dependencies/stb_image.h>
#include <GLFW/glfw3.h>
#include <engine/core/include/debug.hpp>
#include <vector>
#include <string>

class Texture {
public:
    Texture(const char* textureimage);
    void BindTexture() const;
private:
    GLuint m_handle;
};