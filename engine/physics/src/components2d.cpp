#include <engine/physics/include/components2d.hpp>

PhysicalProperties::PhysicalProperties(float mass, float friction, float restitution): mass(mass), inversemass(1.0f/mass), friction(friction), restitution(restitution) {}

RigidBody2D::RigidBody2D(size_t id, glm::vec2 position, PhysicalProperties properties): properties(properties), position(position), handle(id) {}

void RigidBody2D::ApplyForce(const glm::vec2& force) {
    m_forceaccumulator += force;
    DebugPrint("RigidBody2D", std::to_string(force.x));
}

glm::vec2 Collider2D::transformpoint(glm::vec2 point, glm::vec2 position) const {
    return point * half_extents + position;
}