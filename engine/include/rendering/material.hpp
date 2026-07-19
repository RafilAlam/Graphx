#pragma once

#include <engine/include/rendering/shader.hpp>
#include <glm/glm.hpp>

class Material  {
public:
    Material(Shader& shader);
    Shader& GetShader() const;
    glm::vec4 baseColor = {1.0f, 1.0f, 1.0f, 1.0f};
private:
    Shader* m_shader;
};