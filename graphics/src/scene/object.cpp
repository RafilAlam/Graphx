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

Object::Object(std::string name, const Mesh& mesh, const Material& material)
 : name(name), m_mesh(&mesh), m_material(&material)
 {}

Object::Object(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody)
 : name(name), m_mesh(&mesh), m_material(&material), rigidbody(std::make_unique<RigidBody>(std::move(rigidbody)))
 {}

Object::Object(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody, Collider& collider)
 : name(name), m_mesh(&mesh), m_material(&material), rigidbody(std::make_unique<RigidBody>(std::move(rigidbody))), collider(std::make_unique<Collider>(std::move(collider)))
 {}

const Mesh& Object::GetMesh() const {
    return *m_mesh;
};

const Material& Object::GetMaterial() const {
    return *m_material;
};