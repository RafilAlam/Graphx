#pragma once

#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <glm/glm.hpp>

class Material  {
public:
    Material(Shader& shader, Texture texture);
    Shader& GetShader() const;
    glm::vec4 basecolor = {1.0f, 1.0f, 1.0f, 1.0f};
    Texture texture;
private:
    Shader* m_shader;
};