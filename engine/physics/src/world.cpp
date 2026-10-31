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

bool ConstraintHandler2D::_SAT(const Collider2D& cA, const Collider2D& cB) const {
    struct projection {
        projection(const Collider2D& c, const RigidBody2D& b, glm::vec2 reference) {
            auto shape = primitives::unitshapes[static_cast<size_t>(c.type)];
            for (glm::vec2 point : shape) {
                float dot = glm::dot(c.transformpoint(point, b.position), reference);
                if (dot < min)
                    min = dot;
                if (dot > max)
                    max = dot;
            }
        }
        bool overlap(projection other) {
            return min < other.max && max > other.min;
        }
        float min{std::numeric_limits<float>::max()};
        float max{std::numeric_limits<float>::lowest()};
    };

    auto sA = primitives::unitshapes[static_cast<size_t>(cA.type)];
    const RigidBody2D& rbA = world.rigidbodies[cA.handle.id];
    const RigidBody2D& rbB = world.rigidbodies[cB.handle.id];
    size_t prev_idx = sA.size() - 1;
    for (size_t current_idx=0; current_idx<sA.size(); ++current_idx) {
        glm::vec2 reference = glm::normalize(cA.transformpoint(sA[current_idx], rbA.position) - cA.transformpoint(sA[prev_idx], rbA.position));
        reference = {reference.y, -reference.x};
        
        projection pA(cA, rbA, reference);
        projection pB(cB, rbB, reference);
        if (!pA.overlap(pB)) {
            return false;
        }

        prev_idx = current_idx;
    }

    auto sB = primitives::unitshapes[static_cast<size_t>(cB.type)];
    prev_idx = sB.size() - 1;
    for (size_t current_idx=0; current_idx<sB.size(); ++current_idx) {
        glm::vec2 reference = glm::normalize(cB.transformpoint(sB[current_idx], rbB.position) - cB.transformpoint(sB[prev_idx], rbB.position));
        reference = {reference.y, -reference.x};

        projection pA(cA, rbA, reference);
        projection pB(cB, rbB, reference);
        if (!pA.overlap(pB)) {
            return false;
        }

        prev_idx = current_idx;
    }

    return true;
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