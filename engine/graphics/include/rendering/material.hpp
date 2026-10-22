#pragma once

#include <engine/core/include/common.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <glm/glm.hpp>

class Material  {
public:
    Material(Shader& shader, Texture texture, float friction, float restitution);
    Shader& GetShader() const;
    glm::vec4 basecolor = {1.0f, 1.0f, 1.0f, 1.0f};
    AssetHandle texture;
private:
    Shader* m_shader;
};