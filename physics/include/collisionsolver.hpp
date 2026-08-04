#pragma once


#include <graphics/include/scene/object.hpp>
#include <glm/glm.hpp>

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    float penetrationDepth{0.0f};
    glm::vec3 contactPoint{0.0f};
};

struct Contact {
    Object& A;
    Object& B;

    CollisionManifold manifold;

    float accumulatedimpulse{0.0f};
    float K;
    float bias;
    float gamma;
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