#include <engine/graphics/include/scene/model.hpp>

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

Object::Object(std::string name, const StaticMesh& mesh, const Material& material)
 : name(name), m_mesh(&mesh), m_material(&material)
 {}

Object::Object(std::string name, const StaticMesh& mesh, const Material& material, RigidBody& rigidbody)
 : name(name), m_mesh(&mesh), m_material(&material), rigidbody(std::make_unique<RigidBody>(std::move(rigidbody)))
 {}

Object::Object(std::string name, const StaticMesh& mesh, const Material& material, RigidBody& rigidbody, Collider& collider)
 : name(name), m_mesh(&mesh), m_material(&material), rigidbody(std::make_unique<RigidBody>(std::move(rigidbody))), collider(std::make_unique<Collider>(std::move(collider)))
 {}

const StaticMesh& Object::GetMesh() const {
    return *m_mesh;
};

const Material& Object::GetMaterial() const {
    return *m_material;
};

Model::Model(uint64_t id, std::string& name) : id(id), name(name) {}

glm::vec3 Model::GetPosition() {
    glm::vec3 sumposition{0.0f};

    if (objects.empty())
        return sumposition;

    for (auto& objectpair : objects) {
        Object& object = *objectpair.second;
        sumposition += object.rigidbody->position;
    }
    return sumposition / static_cast<float>(objects.size());
}

void Model::SetPosition(glm::vec3 newposition) {
    position = newposition;
    if (objects.empty())
        return;

    const glm::vec3 pivot = GetPosition();
    glm::vec3 delta = newposition - pivot;

    for (auto& objectpair : objects) {
        Object& object = *objectpair.second;
        object.rigidbody->position += delta;
        object.rigidbody->Wake();
    }
}

glm::vec3 Object::PointToWorldSpace(glm::vec3 localpoint) {
    return transform.rotation * localpoint + transform.position;
}