#include <engine/physics/include/components2d.hpp>

PhysicalProperties::PhysicalProperties(float mass, float friction, float restitution): mass(mass), inversemass(1.0f/mass), friction(friction), restitution(restitution) {}

RigidBody2D::RigidBody2D(size_t id, glm::vec2 position, PhysicalProperties properties): properties(properties), position(position), handle(id) {}

void RigidBody2D::ApplyForce(const glm::vec2& force) {
    if (properties.mass <= 0)
        return;

    m_forceaccumulator += force;
    Debug::Print("RigidBody2D", std::to_string(force.x));
}

void RigidBody2D::ApplyImpulse(const glm::vec2& impulse) {
    if (properties.mass <= 0)
        return;

    linearvelocity += impulse * properties.inversemass;
}

Collider2D::Collider2D(size_t id, glm::vec2 half_extents): handle(id), half_extents(half_extents) {};

glm::vec2 Collider2D::transformpoint(glm::vec2 point, glm::vec2 position) const {
    return point * half_extents + position;
}