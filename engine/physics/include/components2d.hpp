#pragma once

#include <glm/glm.hpp>
#include <engine/core/include/debug.hpp>
#include <limits>
#include <span>
#include <string>
// Defines components that make up an entity

class World;

struct PhysicsHandle {
    size_t id;
};

struct PhysicalProperties {
    PhysicalProperties(float mass, float friction, float restitution);
    float mass{1.0f};
    float inversemass{1.0f};
    float friction{1.0f};
    float restitution{1.0f};
};

struct RigidBody2D {
    friend class World;

    RigidBody2D(size_t id, glm::vec2 position, PhysicalProperties properties);
    PhysicalProperties properties;

    glm::vec2 position{0.0f};
    glm::vec2 linearvelocity{0.0f};
    PhysicsHandle handle;

    void ApplyForce(const glm::vec2& force);
    void ApplyImpulse(const glm::vec2& impulse);
private:
    glm::vec2 m_forceaccumulator{0.0f};
};

namespace primitives {
    enum class type {
        Box = 0,
        MaxTypes
    };

    constexpr std::array<glm::vec2, 4> box {
        glm::vec2{-1.0f, -1.0f},
        glm::vec2{1.0f, -1.0f},
        glm::vec2{1.0f, 1.0f},
        glm::vec2{-1.0f, 1.0f}
    };

    inline const std::array<std::span<const glm::vec2>, static_cast<size_t>(type::MaxTypes)> unitshapes {
        std::span<const glm::vec2>{box}
    };
}

struct Collider2D {
    Collider2D(size_t id, glm::vec2 half_extents);
    PhysicsHandle handle;
    glm::vec2 half_extents{1.0f};
    primitives::type type{primitives::type::Box};
    glm::vec2 transformpoint(glm::vec2 point, glm::vec2 position) const;
};