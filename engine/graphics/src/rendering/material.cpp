#include <engine/graphics/include/rendering/material.hpp>

Material::Material(Shader& shader, Texture texture, float friction, float restitution)
 : m_shader(&shader), texture(texture), friction(friction), restitution(restitution)
{}

Shader& Material::GetShader() const {
    return *m_shader;
}