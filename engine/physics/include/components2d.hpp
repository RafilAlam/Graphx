#pragma once

#include <glm/glm.hpp>
#include <engine/core/include/debug.hpp>
#include <string>
// Defines components that make up an entity

class World;

struct PhysicsHandle {
    size_t id;
};

struct PhysicalProperties {
    PhysicalProperties(float mass, float friction, float restitution, glm::vec2 half_extents);
    float mass{1.0f};
    float inversemass{1.0f};
    float friction{1.0f};
    float restitution{1.0f};
    glm::vec2 half_extents{1.0f};
};

struct RigidBody2D {
    friend class World;

    RigidBody2D(size_t id, glm::vec2 position, PhysicalProperties properties);
    PhysicalProperties properties;

    glm::vec2 position{0.0f};
    glm::vec2 linearvelocity{0.0f};
    PhysicsHandle handle;

    void ApplyForce(const glm::vec2& force);
    void ApplyImpulse();
private:
    glm::vec2 m_forceaccumulator{0.0f};
};

struct Collider2D;