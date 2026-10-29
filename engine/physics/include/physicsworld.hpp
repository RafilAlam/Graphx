#pragma once

#include <engine/graphics/include/project/entity.hpp>
#include <engine/graphics/include/project/project.hpp>
#include <engine/physics/include/joint.hpp>
#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <engine/physics/include/integrator.hpp>
#include <engine/physics/include/collisionsolver.hpp>
#include <engine/physics/include/collider.hpp>
#include <unordered_map>
#include <memory>
#include <deque>
#include <vector>
#include <stack>
#include <stdint.h>

bool SameObjectPair(ContactID& id, Contact& contact);

void UpdateRestTime(Object& object, float dt);
bool IsSleepy(Object& object);

struct Island {
    std::vector<Contact*> contacts;
    std::vector<RigidBody*> rigidbodies;
    bool sleeping{false};
};

class PhysicsWorld {
public:
    PhysicsWorld(Scene& scene);

    template<typename T>
    void SetIntegrator() {
        m_integrator = std::make_unique<T>();
    }

    void Step(float deltaTime);

    void UpdateCollider(EntityHandle handle);

    float gravity{-981.0f};

    std::vector<std::unique_ptr<Contact>> m_contacts;
    
    void AddContactEdge(RigidBody& rigidbody, ContactEdge* contactedge);
    void UpdateContactGraph(Contact& contact);

    std::vector<std::unique_ptr<Island>> m_islands;
    void CreateIsland(RigidBody& rigidbody);

private:
    Scene& m_scene;
    std::unique_ptr<BaseIntegrator> m_integrator{std::make_unique<EulerIntegrator>()};
    CollisionSolver m_collisionsolver;
};