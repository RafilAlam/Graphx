#include <engine/physics/include/world.hpp>
#include <engine/graphics/include/rendering/debugdrawbuffer.hpp>

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

CollisionManifold::CollisionManifold() {
    contacts.reserve(2);
}

CollisionManifold ConstraintHandler2D::test_sat(const Collider2D& cA, const Collider2D& cB) const {
    using edge = std::array<glm::vec2, 2>;
    struct clippededge {
        std::array<glm::vec2, 2> points;
        size_t count{0};
        const bool empty() {
            return count == 0;
        }
    };
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
        const float overlap(projection other) {
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
    auto clip = [](glm::vec2 clippingedge, glm::vec2 referencepoint, clippededge input) {
        if (input.empty())
            return input;

        if (input.count == 1) {
            float s = glm::dot(clippingedge, input.points[0]) - glm::dot(clippingedge, referencepoint);

            if (s >= 0.0f) {
                return clippededge{{input.points[0]}, 1};
            }

            return clippededge{};
        }

        float s1 = glm::dot(clippingedge, input.points[0]) - glm::dot(clippingedge, referencepoint);
        float s2 = glm::dot(clippingedge, input.points[1]) - glm::dot(clippingedge, referencepoint);
        
        bool inside0 = s1 <= 0;
        bool inside1 = s2 <= 0;

        if (inside0 && inside1)
            return input;
        if (!inside0 && !inside1) {
            Debug::Print("Returned Clip");
            return clippededge{};
        }

        float t = s1 / (s1 - s2);
        glm::vec2 intersection = input.points[0] + t * (input.points[1] - input.points[0]);
        
        return inside0
        ? clippededge{{input.points[0], intersection}, 2}
        : clippededge{{intersection, input.points[1]}, 2};
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

    // Find Reference Edge in A
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

    // Find Reference Edge in B
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

    glm::vec2 refdirection = glm::normalize(reference[0] - reference[1]);
    
    // detected reference and incident points
    DebugDrawBuffer::AddLine(reference[0], reference[1], color::red);
    DebugDrawBuffer::AddLine(incident[0], incident[1], color::blue);

    // Clipping
    clippededge clipped = {incident, 2};
    clipped = clip(refdirection, reference[0], clipped);
    clipped = clip(-refdirection, reference[1], clipped);
    if (clipped.empty())
        return manifold;
    
    float clippingedge2 = glm::dot(manifold.normal, reference[0]);
    for (size_t i=0; i<clipped.count; ++i) {
        float depth = glm::dot(manifold.normal, clipped.points[i]) - clippingedge2;
        if (depth < 0)
            manifold.contacts.emplace_back(clipped.points[i], depth);
    }

    manifold.colliding = !manifold.contacts.empty();
    return manifold;
}

float ConstraintHandler2D::combine_friction(float a, float b) {
    return a * b;
}

float ConstraintHandler2D::combine_restitution(float a, float b) {
    return std::max(a, b);
}

void ConstraintHandler2D::solve_collision(CollisionManifold manifold) {
    auto crossfv2 = [](float a, const glm::vec2& b) {
        return glm::vec2(-a * b.y, a * b.x);
    };
    auto cross2D = [](const glm::vec2& a,  const glm::vec2& b) {
        return (a.x * b.y) - (a.y * b.x);
    };
    RigidBody2D& rbA = world.rigidbodies[manifold.reference.id];
    RigidBody2D& rbB = world.rigidbodies[manifold.incident.id];
    float restitution = combine_restitution(rbA.properties.restitution, rbB.properties.restitution);
    float friction = combine_friction(rbA.properties.friction, rbB.properties.friction);

    for (const auto& contact : manifold.contacts) {
        glm::vec2 rA = contact.position - rbA.position;
        glm::vec2 rB = contact.position - rbB.position;

        glm::vec2 vAcontact = rbA.linearvelocity + crossfv2(rbA.angularvelocity, rA);
        glm::vec2 vBcontact = rbB.linearvelocity + crossfv2(rbB.angularvelocity, rB);

        glm::vec2 tangent = {-manifold.normal.y, manifold.normal.x};

        float vRelAlongNormal = glm::dot(vBcontact - vAcontact, manifold.normal);
        if (vRelAlongNormal > 0.0f)
            continue;

        float normalvDelta = -(1.0f + restitution) * vRelAlongNormal;
        float normalK = rbA.properties.inversemass + rbB.properties.inversemass 
                  + rbA.properties.inverseinertia * std::pow(cross2D(rA, manifold.normal), 2) 
                  + rbB.properties.inverseinertia * std::pow(cross2D(rB, manifold.normal), 2);

        if (normalK == 0.0f)
            continue;

        float normalimpulse = std::max(0.0f, normalvDelta / normalK);

        float vRelAlongTangent = glm::dot(vBcontact - vAcontact, tangent);
        float tangentvDelta = -vRelAlongTangent;
        float tangentK = rbA.properties.inversemass + rbB.properties.inversemass 
                       + rbA.properties.inverseinertia * std::pow(cross2D(rA, tangent), 2) 
                       + rbB.properties.inverseinertia * std::pow(cross2D(rB, tangent), 2);
        float tangentimpulse = std::max(-friction*normalimpulse, std::min(friction*normalimpulse, tangentvDelta / tangentK));

        glm::vec2 impulse = normalimpulse * manifold.normal + tangentimpulse * tangent;

        rbA.ApplyImpulseAtPosition(contact.position, -impulse);
        rbB.ApplyImpulseAtPosition(contact.position, impulse);
    }
}

void ConstraintHandler2D::HandleCollisions() {
    manifolds.clear();
    for (size_t i=0; i<world.colliders.dense.size(); ++i) {
        for (size_t j=i+1; j<world.colliders.dense.size(); ++j) {
            if (!test_aabb(world.colliders.dense[i], world.colliders.dense[j]))
                continue;
            CollisionManifold manifold = test_sat(world.colliders.dense[i], world.colliders.dense[j]);
            if (manifold.colliding) {
                DebugDrawBuffer::AddPoint(manifold.contacts[0].position, color::yellow);
                DebugDrawBuffer::AddPoint(manifold.contacts[1].position, color::yellow);
                manifolds.emplace_back(std::move(manifold));
            }
        }
    }

    for (const auto& manifold : manifolds) {
        solve_collision(manifold);
    }
}

World::World(): constrainthandler(*this) {}

void World::integratevelocity(RigidBody2D& rigidbody, float dt) {
    if (rigidbody.properties.mass <= 0)
        return;

    rigidbody.linearvelocity += rigidbody.m_forceaccumulator * rigidbody.properties.inversemass * dt;
    rigidbody.m_forceaccumulator = glm::vec2(0.0f);
}
void World::integrateposition(RigidBody2D& rigidbody, float dt) {
    if (rigidbody.properties.mass <= 0)
        return;

    rigidbody.position += rigidbody.linearvelocity * dt;
}

void World::Simulate(float dt) {
    for (auto& rigidbody : rigidbodies.dense) {
        if (rigidbody.properties.mass <= 0)
            continue;

        integratevelocity(rigidbody, dt);
        rigidbody.linearvelocity -= glm::vec2{0.0f, settings.gravity} * dt;
    }
    constrainthandler.HandleCollisions();
    for (auto& rigidbody : rigidbodies.dense) {
        integrateposition(rigidbody, dt);
    }
}