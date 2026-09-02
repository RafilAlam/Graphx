#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/core/include/utils.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>

struct ContactID {
    size_t referenceIt;
    size_t incidentIt;

    bool operator==(const ContactID&) const = default;
};

struct ContactPoint {
    ContactID id;
    glm::vec3 position;
    float normalImpulse;
};

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    float penetrationDepth{0.0f};
    std::vector<ContactPoint> contactPoints;
};

struct Contact {    
    Object& A;
    Object& B;

    CollisionManifold manifold;
};

bool ClipSegmentToLine(std::vector<ContactPoint>& incidentFace, const glm::vec3& clipnormal, float offset);

Contact CircleCircleCheck(Object& A, Object& B);
Contact RectangleCircleCheck(RigidBody& A, RigidBody& B);
Contact PolygonPolygonCheck(Object& A, Object& B);

class CollisionSolver {
public:
    CollisionSolver();
    
    using CollisionFn = Contact(*)(Object&, Object&);
    CollisionFn Dispatch[static_cast<int>(ColliderType::Count)][static_cast<int>(ColliderType::Count)];

    void PreStep(Contact& contact, float deltaTime);
    void Resolve(Contact& contact);
};