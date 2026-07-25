#pragma once

#include <physics/include/rigidbody.hpp>
#include <glm/glm.hpp>

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    float penetrationDepth{0.0f};
};

struct Contact {
    RigidBody* A;
    RigidBody* B;

    CollisionManifold manifold;
};

Contact CircleCircleCheck(RigidBody& A, RigidBody& B);
Contact RectangleCircleCheck(RigidBody& A, RigidBody& B);

class CollisionSolver {
public:
    CollisionSolver();
    
    using CollisionFn = Contact(*)(RigidBody&, RigidBody&);
    CollisionFn Dispatch[static_cast<int>(ColliderType::Count)][static_cast<int>(ColliderType::Count)];

    void Resolve(Contact& contact);
};