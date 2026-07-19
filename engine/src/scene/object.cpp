#include <engine/include/scene/object.hpp>

Object::Object(const Mesh& mesh, const Material& material)
 : m_mesh(&mesh), m_material(&material)
 {}

const Mesh& Object::GetMesh() {
    return *m_mesh;
};

const Material& Object::GetMaterial() {
    return *m_material;
};