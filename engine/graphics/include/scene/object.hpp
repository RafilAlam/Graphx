#pragma once

#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/physics/include/rigidbody.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    void SetRotation(glm::vec3 degrees);
    void SetRotation(glm::vec3 axis, float degrees);
    void Rotate(glm::vec3 degrees);
    void Rotate(glm::vec3 axis, float degrees);
};

class Object {
public:
    Object(std::string name, const Mesh& mesh, const Material& material);
    Object(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody);
    Object(std::string name, const Mesh& mesh, const Material& material, RigidBody& rigidbody, Collider& collider);
    const Mesh& GetMesh() const;
    const Material& GetMaterial() const;
    Transform transform;
    std::unique_ptr<RigidBody> rigidbody;
    std::unique_ptr<Collider> collider;
    std::string name;
private:
    const Mesh* m_mesh;
    const Material* m_material;
};