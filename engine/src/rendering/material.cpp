#include <engine/include/rendering/material.hpp>

Material::Material(Shader& shader, Texture texture)
 : m_shader(&shader), texture(texture)
{}

Shader& Material::GetShader() const {
    return *m_shader;
}