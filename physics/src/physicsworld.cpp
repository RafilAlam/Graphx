#include <physics/include/physicsworld.hpp>

RigidBody& PhysicsWorld::CreateRigidBody(float Mass) {
    return m_rigidbodies.emplace_back(Mass);
}

void PhysicsWorld::Step(float deltaTime) {
    for (auto& body : m_rigidbodies) {
        m_integrator->Integrate(body, deltaTime);
    }
}