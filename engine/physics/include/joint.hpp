#pragma once

#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/project/entity.hpp>
#include <glm/glm.hpp>

#include <numbers>

class JointVisuals;

class Joint {
public:
    Joint(std::string name, Object* ObjectA, glm::vec3 localanchorA, Object* ObjectB, glm::vec3 localanchorB);
    virtual void SolveJoint(float deltaTime) = 0;

    std::string name;
    Object* ObjectA;
    glm::vec3 localanchorA;
    Object* ObjectB;
    glm::vec3 localanchorB;

    glm::vec3 anchorA{0.0f};
    glm::vec3 anchorB{0.0f};

    std::unique_ptr<JointVisuals> visuals;
};

class JointVisuals {
public:
    JointVisuals(Joint* joint, StaticMesh& mesh, Material& material);
    virtual void Update() = 0;
    Joint* joint;
    StaticMesh& mesh;
    Material& material;
    Transform transform;
};

class RodVisuals : public JointVisuals {
public:
    RodVisuals(Joint* joint, StaticMesh& mesh, Material& material);
    void Update() override;
};

class RodJoint : public Joint {
public:
    RodJoint(std::string name, Object* ObjectA, glm::vec3 localanchorA, Object* ObjectB, glm::vec3 localanchorB, float length, StaticMesh& mesh, Material& material);
    void SolveJoint(float deltaTime) override;

    float length;
};