#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/core/include/services.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <cmath>

struct ObjectPair {
    Object* Object1;
    Object* Object2;

    bool operator==(const ObjectPair& other) const {
        return (Object1 == other.Object1 && Object2 == other.Object2) || (Object1 == other.Object2 && Object2 == other.Object1);
    }
};

enum class FeatureType {
    Vertex,
    Face
};

struct Feature {
    size_t index;
    FeatureType type;

    bool operator==(const Feature&) const = default;
};

struct ContactID {
    ObjectPair objectpair;
    Feature feature1;
    Feature feature2;

    bool operator==(const ContactID&) const = default;
};

struct ContactPoint {
    ContactID id;
    glm::vec3 position;
    float penetrationDepth;
    float normalImpulse;
};

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    std::vector<ContactPoint> contactPoints;
};

struct Contact {    
    Object& reference;
    Object& incident;

    CollisionManifold manifold;
};

bool ClipSegmentToLine(std::vector<ContactPoint>& points, const glm::vec3& normal, glm::vec3 referencePoint, size_t referenceVertexIndex, size_t incidentFaceIndex);

Contact CircleCircleCheck(Object& A, Object& B);
Contact RectangleCircleCheck(RigidBody& A, RigidBody& B);
Contact PolygonPolygonCheck(Object& A, Object& B);

class CollisionSolver {
public:
    CollisionSolver();
    
    using CollisionFn = Contact(*)(Object&, Object&);
    CollisionFn Dispatch[static_cast<int>(ColliderType::Count)][static_cast<int>(ColliderType::Count)];

    void WarmStart(Contact& contact);
    void Resolve(Contact& contact, float deltaTime);
    void PositionCorrection(Contact& contact, float dt);
};