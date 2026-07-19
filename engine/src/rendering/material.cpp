#include <engine/include/rendering/material.hpp>

Material::Material(Shader& shader)
 : m_shader(&shader)
{}

Shader& Material::GetShader() const {
    return *m_shader;
}