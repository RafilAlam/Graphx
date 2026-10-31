#include <engine/physics/include/world.hpp>

bool ConstraintHandler2D::_AABB(const Collider2D& cA, const Collider2D& cB) const {
    struct projection {
        glm::vec2 min;
        glm::vec2 max;
        bool overlap(projection other) const {
            return (min.x <= other.max.x && max.x >= other.min.x) && 
                   (min.y <= other.max.y && max.y >= other.min.y); 
        }
    };

    const RigidBody2D& rbA = world.rigidbodies[cA.handle.id];
    projection pA{
        .min = rbA.position - cA.half_extents,
        .max = rbA.position + cA.half_extents
    };

    const RigidBody2D& rbB = world.rigidbodies[cB.handle.id];
    projection pB{
        .min = rbB.position - cB.half_extents,
        .max = rbB.position + cB.half_extents
    };

    return pA.overlap(pB);
}

void ConstraintHandler2D::HandleCollision(const Collider2D& cA, const Collider2D& cB) {
    if (!_AABB(cA, cB))
        return;
}

void World::integratevelocity(RigidBody2D& rigidbody, float dt) {
    rigidbody.linearvelocity += rigidbody.m_forceaccumulator * rigidbody.properties.inversemass * dt;
    rigidbody.m_forceaccumulator = glm::vec2(0.0f);
}
void World::integrateposition(RigidBody2D& rigidbody, float dt) {
    rigidbody.position += rigidbody.linearvelocity * dt;
}

void World::Simulate(float dt) {
    for (auto& rigidbody : rigidbodies.dense) {
        integratevelocity(rigidbody, dt);
        integrateposition(rigidbody, dt);
        rigidbody.linearvelocity -= glm::vec2{0.0f, settings.gravity};
    }
}