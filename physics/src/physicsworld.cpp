#include <physics/include/physicsworld.hpp>
#include <iostream>

RigidBody& PhysicsWorld::CreateRigidBody(float mass, Collider& collider) {
    return m_rigidbodies.emplace_back(mass, collider);
}

void PhysicsWorld::Step(float deltaTime) {
    
    for (int i = 0; i < m_rigidbodies.size(); ++i) {
        m_integrator->Integrate(m_rigidbodies[i], deltaTime);
        for (int j = i + 1; j < m_rigidbodies.size(); ++j) {
            Contact contact = m_collisionsolver.Dispatch[m_rigidbodies[i].collider.type][m_rigidbodies[j].collider.type](m_rigidbodies[i], m_rigidbodies[j]);
            m_collisionsolver.Resolve(contact);
            if (contact.manifold.colliding) {
                std::cout << "Collision Detected!" << '\n';
            }
        }
    }
}