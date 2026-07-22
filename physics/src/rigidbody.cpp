#include <physics/include/rigidbody.hpp>

RigidBody::RigidBody(float Mass) : mass(Mass), m_inversemass(1/Mass) {}

void RigidBody::ApplyImpulse(glm::vec3 impulsevector) {
    velocity += impulsevector * m_inversemass;
}