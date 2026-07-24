#include <physics/include/rigidbody.hpp>

RigidBody::RigidBody(float Mass) : mass(Mass), m_inversemass(1/Mass) {}

void RigidBody::ApplyImpulse(glm::vec3 impulsevector) {
    velocity += impulsevector * m_inversemass;
}

void RigidBody::ApplyForce(glm::vec3 forcevector) {
    m_accumulatedforce += forcevector;
}

glm::vec3 RigidBody::GetAccumulatedForce() {
    return m_accumulatedforce;
}

float RigidBody::GetInverseMass() {
    return m_inversemass;
}