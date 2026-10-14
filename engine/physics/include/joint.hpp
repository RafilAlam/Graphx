#pragma once

#include <engine/graphics/include/scene/model.hpp>
#include <glm/glm.hpp>

class Joint {
public:
    virtual void SolveJoint(float deltaTime) = 0;
};

class RodJoint : public Joint {
public:
    RodJoint(std::string name, Object& ObjectA, glm::vec3 localanchorA, Object& ObjectB, glm::vec3 localanchorB, float length);
    void SolveJoint(float deltaTime) override;

    uint64_t id;
    std::string name;
    Object* ObjectA;
    glm::vec3 localanchorA;
    Object* ObjectB;
    glm::vec3 localanchorB;
    float length;
};