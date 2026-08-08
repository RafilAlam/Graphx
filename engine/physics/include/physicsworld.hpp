#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <engine/physics/include/integrator.hpp>
#include <engine/physics/include/collisionsolver.hpp>
#include <engine/physics/include/collider.hpp>
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