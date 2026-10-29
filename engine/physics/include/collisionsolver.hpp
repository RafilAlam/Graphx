#pragma once

#include <engine/graphics/include/project/project.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/core/include/services.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <cmath>

bool ClipSegmentToLine(std::vector<ContactPoint>& points, const glm::vec3& normal, glm::vec3 referencePoint, size_t referenceVertexIndex, size_t incidentFaceIndex);
float CombineFriction(float A, float B);
float CombineRestitution(float A, float B);

struct EntityPhysicsData {
    Transform& transform;
    RigidBody& rigidbody;
    Collider& collider;
};

bool AABBCheck(EntityPhysicsData& A, EntityPhysicsData& B);
Contact CircleCircleCheck(EntityPhysicsData& A, EntityPhysicsData& B);
Contact PolygonPolygonCheck(EntityPhysicsData& A, EntityPhysicsData& B);

class CollisionSolver {
public:
    CollisionSolver(Scene& scene);
    
    using CollisionFn = Contact(*)(EntityHandle, EntityHandle);
    CollisionFn Dispatch[static_cast<int>(ColliderType::Count)][static_cast<int>(ColliderType::Count)];

    void PreSolve(Contact& contact);
    void WarmStart(Contact& contact);
    void Resolve(Contact& contact, float deltaTime);
    void PositionCorrection(Contact& contact, float dt);

private:
    Scene& m_scene;
};