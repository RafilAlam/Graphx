#pragma once

#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <glm/glm.hpp>

class Material  {
public:
    Material(Shader& shader, Texture texture, float friction, float restitution);
    Shader& GetShader() const;
    glm::vec4 basecolor = {1.0f, 1.0f, 1.0f, 1.0f};
    Texture texture;
    float friction{0.0f};
    float restitution{0.0f};
private:
    Shader* m_shader;
};