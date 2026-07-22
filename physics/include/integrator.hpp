#pragma once

#include <physics/include/rigidbody.hpp>

class BaseIntegrator {
public:
    BaseIntegrator() = default;
    ~BaseIntegrator() = default;

    virtual void Integrate(RigidBody& body, float deltaTime) = 0;
};

class EulerIntegrator : public BaseIntegrator {
public:
    void Integrate(RigidBody& body, float deltaTime) override;
};