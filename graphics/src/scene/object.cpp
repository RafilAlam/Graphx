#include <graphics/include/scene/object.hpp>

void Transform::SetRotation(glm::vec3 degrees) {
    rotation = glm::quat(glm::radians(degrees));
}

void Transform::SetRotation(glm::vec3 axis, float degrees) {
    rotation = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
}

void Transform::Rotate(glm::vec3 degrees) {
    glm::quat delta = glm::quat(glm::radians(degrees));
    rotation *= delta;
}

void Transform::Rotate(glm::vec3 axis, float degrees) {
    rotation *= glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
}

Object::Object(const Mesh& mesh, const Material& material)
 : m_mesh(&mesh), m_material(&material)
 {}

const Mesh& Object::GetMesh() const {
    return *m_mesh;
};

const Material& Object::GetMaterial() const {
    return *m_material;
};