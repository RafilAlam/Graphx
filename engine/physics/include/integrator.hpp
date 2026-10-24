#pragma once

#include <engine/physics/include/rigidbody.hpp>

class BaseIntegrator {
public:
    BaseIntegrator() = default;
    ~BaseIntegrator() = default;

    virtual void IntegrateVelocity(RigidBody& body, float deltaTime) = 0;
    virtual void IntegrateTransform(RigidBody& body, float deltaTime) = 0;
};

class EulerIntegrator : public BaseIntegrator {
public:
    void IntegrateVelocity(RigidBody& body, float deltaTime) override;
    void IntegrateTransform(RigidBody& body, float deltaTime) override;
};