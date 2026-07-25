#pragma once

#include <physics/include/rigidbody.hpp>
#include <physics/include/integrator.hpp>
#include <physics/include/collisionsolver.hpp>
#include <physics/include/collider.hpp>
#include <memory>
#include <deque>

class PhysicsWorld {
public:
    PhysicsWorld() = default;
    RigidBody& CreateRigidBody(float mass, Collider& collider);

    template<typename T>
    void SetIntegrator() {
        m_integrator = std::make_unique<T>();
    }

    void Step(float deltaTime);

private:
    std::unique_ptr<BaseIntegrator> m_integrator{std::make_unique<EulerIntegrator>()};
    CollisionSolver m_collisionsolver;
    std::deque<RigidBody> m_rigidbodies;
};