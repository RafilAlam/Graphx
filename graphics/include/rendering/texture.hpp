#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/include/debug.hpp>
#include <vector>
#include <string>
#include <dependencies/stb_image.h>

class Texture {
public:
    Texture(const char* textureimage);
    void BindTexture() const;
private:
    GLuint m_handle;
};