#include <engine/physics/include/rigidbody.hpp>

RigidBody::RigidBody(float mass) : mass(mass), m_inversemass(mass==0.0f ? 0.0f : 1.0f/mass) {}

void RigidBody::ApplyImpulse(glm::vec3 impulsevector) {
    velocity += impulsevector * m_inversemass;
}

void RigidBody::ApplyForce(glm::vec3 forcevector) {
    m_accumulatedforce += forcevector;
}

glm::vec3 RigidBody::GetAccumulatedForce() {
    return m_accumulatedforce;
}

void RigidBody::ClearAccumulatedForce() {
    m_accumulatedforce = glm::vec3(0.0f);
}

float RigidBody::GetInverseMass() {
    return m_inversemass;
}