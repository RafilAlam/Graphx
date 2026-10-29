#pragma once

#include <engine/physics/include/components2d.hpp>
#include <vector>

// Defines a world/scene where components are processed

struct WorldSettings {
    float gravity{9.81};
};

class World {
public:
    WorldSettings settings;

    std::vector<RigidBody2D> rigidbodies;

    void Simulate(float dt);
private:
    void integratevelocity(RigidBody2D& rigidbody, float dt);
    void integrateposition(RigidBody2D& rigidbody, float dt);
};