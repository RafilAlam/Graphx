#pragma once

#include <graphics/include/scene/object.hpp>

class BaseIntegrator {
public:
    BaseIntegrator() = default;
    ~BaseIntegrator() = default;

    virtual void IntegrateVelocity(RigidBody& body, float deltaTime) = 0;
    virtual void IntegratePosition(RigidBody& body, float deltaTime) = 0;
};

class EulerIntegrator : public BaseIntegrator {
public:
    void IntegrateVelocity(RigidBody& body, float deltaTime) override;
    void IntegratePosition(RigidBody& body, float deltaTime) override;
};