#pragma once

#include <graphics/include/scene/object.hpp>
#include <physics/include/integrator.hpp>
#include <physics/include/collisionsolver.hpp>
#include <physics/include/collider.hpp>
#include <memory>
#include <deque>

class PhysicsWorld {
public:
    PhysicsWorld() = default;

    template<typename T>
    void SetIntegrator() {
        m_integrator = std::make_unique<T>();
    }

    void Step(std::deque<Object>& objects, float deltaTime);

    void UpdateCollider(Object& object);

    float gravity{-981.0f};

private:
    std::unique_ptr<BaseIntegrator> m_integrator{std::make_unique<EulerIntegrator>()};
    CollisionSolver m_collisionsolver;
};