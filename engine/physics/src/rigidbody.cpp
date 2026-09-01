#include <engine/physics/include/rigidbody.hpp>

RigidBody::RigidBody(float mass, glm::vec3 size) : m_size(size), m_mass(mass), m_inertia(mass * (m_size.x * m_size.x + m_size.y * m_size.y) / 12), m_inversemass(mass==0.0f ? 0.0f : 1.0f/mass), m_inverseinertia(m_inertia==0.0f ? 0.0f : 1.0f/m_inertia) {}

void RigidBody::ApplyImpulse(glm::vec3 impulse) {
    velocity += impulse * m_inversemass;
}

void RigidBody::ApplyImpulseAtPosition(glm::vec3 impulse, glm::vec3 impulseposition) {
    velocity += impulse * m_inversemass;
    angularvelocity.z += m_inverseinertia * Cross2D(impulseposition - position, impulse);
}

glm::vec3 RigidBody::GetPointVelocity(glm::vec3 point) {
    glm::vec3 radius = point - position;
    return velocity + glm::cross(angularvelocity, radius);
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

void RigidBody::m_recalculateinertia() {
    m_inertia = m_mass * (m_size.x * m_size.x + m_size.y * m_size.y) / 12;
    m_inverseinertia = m_inertia==0.0f ? 0.0f : 1.0f/m_inertia;
}

void RigidBody::SetSize(glm::vec3 size) {
    m_size = size;
    m_recalculateinertia();
}

void RigidBody::SetMass(float mass) {
    m_mass = mass;
    m_inversemass = mass==0.0f ? 0.0f : 1.0f/mass;
    m_recalculateinertia();
}

float RigidBody::GetMass() {
    return m_mass;
}

float RigidBody::GetInverseMass() {
    return m_inversemass;
}

float RigidBody::GetInertia() {
    return m_inertia;
}

float RigidBody::GetInverseInertia() {
    return m_inverseinertia;
}