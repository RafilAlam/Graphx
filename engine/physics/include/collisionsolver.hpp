#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/core/include/utils.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>

bool ClipSegmentToLine(std::vector<glm::vec3>& incidentFace, const glm::vec3& clipnormal, float offset);

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    float penetrationDepth{0.0f};
    std::vector<glm::vec3> contactPoints;
};

struct Contact {
    Object& A;
    Object& B;

    CollisionManifold manifold;
    float accumulatedimpulses[2];
};

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