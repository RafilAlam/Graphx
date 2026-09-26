#pragma once

#include <engine/graphics/include/scene/object.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/core/include/services.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <vector>
#include <cmath>

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