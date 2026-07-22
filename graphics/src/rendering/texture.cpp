#include <graphics/include/rendering/texture.hpp>

Texture::Texture(const char* textureimage) {
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureimage, &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        DebugError("Failed to load texture");
    }

    stbi_image_free(data);
}

void Texture::BindTexture() const {
    glBindTexture(GL_TEXTURE_2D, m_handle);
}