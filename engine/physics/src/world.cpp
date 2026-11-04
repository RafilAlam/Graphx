#include <engine/physics/include/world.hpp>

ConstraintHandler2D::ConstraintHandler2D(World& world): world(world) {}

bool ConstraintHandler2D::test_aabb(const Collider2D& cA, const Collider2D& cB) const {
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

CollisionManifold::CollisionManifold(): contacts(2) {}

CollisionManifold ConstraintHandler2D::test_sat(const Collider2D& cA, const Collider2D& cB) const {
    using edge = std::array<glm::vec2, 2>;
    struct projection {
        projection(const Collider2D& c, const RigidBody2D& b, glm::vec2 normal) {
            auto shape = primitives::unitshapes[static_cast<size_t>(c.type)];
            for (glm::vec2 point : shape) {
                float dot = glm::dot(c.transformpoint(point, b.position), normal);
                if (dot < min)
                    min = dot;
                if (dot > max)
                    max = dot;
            }
        }
        float overlap(projection other) {
            return std::min(max - other.min, other.max - min);
        }
        float min{std::numeric_limits<float>::max()};
        float max{std::numeric_limits<float>::lowest()};
    };
    auto getIncidentEdge = [](std::span<const glm::vec2> s, const RigidBody2D& rb, const Collider2D& c, glm::vec2 referenceNormal) {
        edge incident;
        float minalign{0.0f};
            size_t prev_idx = s.size() - 1;
        for (size_t current_idx=0; current_idx<s.size(); ++current_idx) {
            glm::vec2 p0 = c.transformpoint(s[current_idx], rb.position);
            glm::vec2 p1 = c.transformpoint(s[prev_idx], rb.position);
            glm::vec2 normal = glm::normalize(p0 - p1);
            normal = {normal.y, -normal.x};
            
            float align = glm::dot(normal, referenceNormal);
            if (align < minalign) {
                incident = {p0, p1};
                minalign = align;
            }

            prev_idx = current_idx;
        }

        return incident;
    };
    auto clip = [](glm::vec2 clippingedge, glm::vec2 referencepoint, edge& points) {
        float s1 = glm::dot(clippingedge, points[0]) - glm::dot(clippingedge, referencepoint);
        float s2 = glm::dot(clippingedge, points[1]) - glm::dot(clippingedge, referencepoint);

        if (s1 < 0 && s2 < 0)
            return 1;
        if (s1 >= 0 && s2 >= 0)
            return 0;

        float t = s1 / (s1 - s2);
        if (s2 >= 0)
            points[0] = points[0] + t * (points[1] - points[0]);
        else
            points[1] = points[0] + t * (points[1] - points[0]);
        
        return 0;
    };

    const RigidBody2D& rbA = world.rigidbodies[cA.handle.id];
    const RigidBody2D& rbB = world.rigidbodies[cB.handle.id];
    CollisionManifold manifold;
    float minoverlap{std::numeric_limits<float>::max()};
    bool sAincident{false};
    edge reference;
    edge incident;

    auto sA = primitives::unitshapes[static_cast<size_t>(cA.type)];
    size_t prev_idx = sA.size() - 1;
    for (size_t current_idx=0; current_idx<sA.size(); ++current_idx) {
        glm::vec2 p0 = cA.transformpoint(sA[current_idx], rbA.position);
        glm::vec2 p1 = cA.transformpoint(sA[prev_idx], rbA.position);
        glm::vec2 normal = glm::normalize(p0 - p1);
        normal = {normal.y, -normal.x};
        
        projection pA(cA, rbA, normal);
        projection pB(cB, rbB, normal);
        float overlap = pA.overlap(pB);
        if (overlap <= 0.0f)
            return manifold;
        if (overlap < minoverlap) {
            reference = {p0, p1};
            manifold.normal = normal;
            minoverlap = overlap;
        }

        prev_idx = current_idx;
    }

    auto sB = primitives::unitshapes[static_cast<size_t>(cB.type)];
    prev_idx = sB.size() - 1;
    for (size_t current_idx=0; current_idx<sB.size(); ++current_idx) {
        glm::vec2 p0 = cB.transformpoint(sB[current_idx], rbB.position);
        glm::vec2 p1 = cB.transformpoint(sB[prev_idx], rbB.position);
        glm::vec2 normal = glm::normalize(p0 - p1);
        normal = {normal.y, -normal.x};

        projection pA(cA, rbA, normal);
        projection pB(cB, rbB, normal);
        float overlap = pA.overlap(pB);
        if (overlap <= 0.0f)
            return manifold;
        if (overlap < minoverlap) {
            reference = {p0, p1};
            manifold.normal = normal;
            minoverlap = overlap;
            sAincident = true;
        }

        prev_idx = current_idx;
    }

    if (sAincident) {
        incident = getIncidentEdge(sA, rbA, cA, manifold.normal);
        manifold.reference = cB.handle;
        manifold.incident = cA.handle;
    }
    else {
        incident = getIncidentEdge(sB, rbB, cB, manifold.normal);
        manifold.reference = cA.handle;
        manifold.incident = cB.handle;
    }

    std::array<glm::vec2, 2> clippedpoints = incident;
    glm::vec2 refdirection = glm::normalize(reference[0] - reference[1]);
    
    if (clip(refdirection, reference[0], clippedpoints) ||
        clip(-refdirection, reference[1], clippedpoints))
        return manifold;

    float clippingedge2 = glm::dot(manifold.normal, reference[0]);
    for (size_t i=0; i<clippedpoints.size(); ++i) {
        float depth = glm::dot(manifold.normal, clippedpoints[i]) - clippingedge2;
        if (depth > 0)
            manifold.contacts.emplace_back(clippedpoints[i], depth);
    }

    manifold.colliding = !manifold.contacts.empty();
    return manifold;
}

void ConstraintHandler2D::solve_collision(CollisionManifold manifold) {
    RigidBody2D& rbA = world.rigidbodies[manifold.reference.id];
    RigidBody2D& rbB = world.rigidbodies[manifold.incident.id];
    float restitution = 1.0f;

    glm::vec2 vRel = rbB.linearvelocity - rbA.linearvelocity;
    glm::vec2 vDelta = -(1 + restitution) * vRel;
    float K = rbA.properties.inversemass + rbB.properties.inversemass;
    glm::vec2 impulse = vDelta / K;

    rbA.ApplyImpulse(impulse);
    rbB.ApplyImpulse(-impulse);
}

void ConstraintHandler2D::HandleCollisions() {
    manifolds.clear();
    for (size_t i=0; i<world.colliders.dense.size(); ++i) {
        for (size_t j=i+1; j<world.colliders.dense.size(); ++j) {
            if (!test_aabb(world.colliders.dense[i], world.colliders.dense[j]))
                continue;
            CollisionManifold manifold = test_sat(world.colliders.dense[i], world.colliders.dense[j]);
            if (manifold.colliding)
                manifolds.emplace_back(std::move(manifold));
        }
    }

    for (const auto& manifold : manifolds) {
        solve_collision(manifold);
    }
}

World::World(): constrainthandler(*this) {}

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
        rigidbody.linearvelocity -= glm::vec2{0.0f, settings.gravity};
    }
    constrainthandler.HandleCollisions();
    for (auto& rigidbody : rigidbodies.dense) {
        integrateposition(rigidbody, dt);
    }
}