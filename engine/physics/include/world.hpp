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
    PhysicsHandle reference;
    PhysicsHandle incident;
};

class ConstraintHandler2D {
    World& world;
    std::vector<CollisionManifold> manifolds;
    bool test_aabb(const Collider2D& cA, const Collider2D& cB) const;
    CollisionManifold test_sat(const Collider2D& cA, const Collider2D& cB) const;
    float combine_friction(float a, float b);
    float combine_restitution(float a, float b);
    void solve_collision(CollisionManifold manifold);
public:
    ConstraintHandler2D(World& world);
    void HandleCollisions();
};

struct WorldSettings {
    float gravity{19.81f};
};

class World {
    void integratevelocity(RigidBody2D& rigidbody, float dt);
    void integrateposition(RigidBody2D& rigidbody, float dt);
public:
    World();
    WorldSettings settings;
    sparseset<RigidBody2D> rigidbodies;
    sparseset<Collider2D> colliders;
    ConstraintHandler2D constrainthandler;
    void Simulate(float dt);
};