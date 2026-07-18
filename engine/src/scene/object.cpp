#include <engine/include/scene/object.hpp>

Object::Object(const Mesh& mesh, const Shader& shader)
 : m_mesh(&mesh), m_shader(&shader)
 {}

const Mesh& Object::GetMesh() {
    return *m_mesh;
};

const Shader& Object::GetShader() {
    return *m_shader;
};