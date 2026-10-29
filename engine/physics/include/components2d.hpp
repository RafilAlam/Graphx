#pragma once

#include <glm/glm.hpp>
// Defines components that make up an entity

class World;

struct PhysicsHandle {
    uint32_t id;
};

struct RigidBody2D {
    friend class World;

    float mass{1.0f};

    glm::vec2 position;
    glm::vec2 linearvelocity;

    void ApplyForce(const glm::vec2& force);
    void ApplyImpulse();
private:
    glm::vec2 m_forceaccumulator;
};