#pragma once

#include <engine/physics/include/components2d.hpp>
#include <engine/core/include/debug.hpp>
#include <engine/core/include/sparseset.hpp>
#include <iostream>
#include <vector>

// Defines a world/scene where components are processed

struct WorldSettings {
    float gravity{9.81f};
};

class World {
public:
    WorldSettings settings;

    sparseset<RigidBody2D> rigidbodies;

    void Simulate(float dt);
private:
    void integratevelocity(RigidBody2D& rigidbody, float dt);
    void integrateposition(RigidBody2D& rigidbody, float dt);
};