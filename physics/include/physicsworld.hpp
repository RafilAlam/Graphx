#pragma once

#include <physics/include/rigidbody.hpp>
#include <physics/include/integrator.hpp>
#include <memory>
#include <deque>

class PhysicsWorld {
public:
    PhysicsWorld() = default;
    RigidBody& CreateRigidBody(float Mass);

    template<typename T>
    void SetIntegrator() {
        m_integrator = std::make_unique<T>();
    }

    void Step(float deltaTime);

private:
    std::unique_ptr<BaseIntegrator> m_integrator{std::make_unique<EulerIntegrator>()};
    std::deque<RigidBody> m_rigidbodies;
};