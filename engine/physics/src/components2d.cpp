#include <engine/physics/include/components2d.hpp>

PhysicalProperties::PhysicalProperties(float mass, glm::vec2 half_extents, float friction, float restitution): mass(mass), inversemass(mass == 0.0f ? 0.0f : 1.0f/mass), inertia(1/3 * mass * (std::pow(half_extents.x, 2) + std::pow(half_extents.y, 2))), inverseinertia(inertia == 0.0f ? 0.0f : 1.0f/inertia), friction(friction), restitution(restitution) {}

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
    DebugDrawBuffer::AddArrow(position, impulse, color::green);
}

void RigidBody2D::ApplyImpulseAtPosition(const glm::vec2& impulseposition, const glm::vec2& impulse) {
    if (properties.mass <= 0)
        return;
    auto cross2D = [](const glm::vec2& a,  const glm::vec2& b) {
        return (a.x * b.y) - (a.y * b.x);
    };
    
    linearvelocity += impulse * properties.inversemass;
    angularvelocity += cross2D(impulseposition - position, impulse) * properties.inverseinertia;
}

Collider2D::Collider2D(size_t id, glm::vec2 half_extents): handle(id), half_extents(half_extents) {};

glm::vec2 Collider2D::transformpoint(glm::vec2 point, glm::vec2 position) const {
    return point * half_extents + position;
}