#include <engine/physics/include/components2d.hpp>

void RigidBody2D::ApplyForce(const glm::vec2& force) {
    m_forceaccumulator += force;
}