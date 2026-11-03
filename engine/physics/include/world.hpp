#pragma once

#include <engine/physics/include/components2d.hpp>
#include <engine/core/include/debug.hpp>
#include <engine/core/include/sparseset.hpp>
#include <iostream>
#include <vector>

// Defines a world/scene where components are processed

struct Contact {
    glm::vec2 position;
    float penetrationdepth;
};

struct CollisionManifold {
    CollisionManifold();
    glm::vec2 normal;
    bool colliding{false};
    std::vector<Contact> contacts;
    PhysicsHandle A;
    PhysicsHandle B;
};

class ConstraintHandler2D {
public:
    void HandleCollision(const Collider2D& cA, const Collider2D& cB);
private:
    World& world;
    bool _AABB(const Collider2D& cA, const Collider2D& cB) const;
    CollisionManifold _SAT(const Collider2D& cA, const Collider2D& cB) const;
};

struct WorldSettings {
    float gravity{9.81f};
};

class World {
public:
    WorldSettings settings;

    sparseset<RigidBody2D> rigidbodies;
    sparseset<Collider2D> colliders;

    ConstraintHandler2D constrainthandler;

    void Simulate(float dt);
private:
    void integratevelocity(RigidBody2D& rigidbody, float dt);
    void integrateposition(RigidBody2D& rigidbody, float dt);
};