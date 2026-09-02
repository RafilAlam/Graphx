#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <engine/physics/include/integrator.hpp>
#include <engine/physics/include/collisionsolver.hpp>
#include <engine/physics/include/collider.hpp>
#include <unordered_map>
#include <memory>
#include <deque>
#include <stdint.h>

struct ShapePair {
    Object* A;
    Object* B;

    bool operator==(const ShapePair&) const = default;
};

struct ShapePairHash {
    size_t operator()(const ShapePair& pair) const {
        size_t h1 = std::hash<uintptr_t>{}((uintptr_t)pair.A);
        size_t h2 = std::hash<uintptr_t>{}((uintptr_t)pair.B);

        return h1 ^ (h2 << 1);
    }
};

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

    std::unordered_map<ShapePair, Contact, ShapePairHash> contacts;

private:
    std::unique_ptr<BaseIntegrator> m_integrator{std::make_unique<EulerIntegrator>()};
    CollisionSolver m_collisionsolver;
};