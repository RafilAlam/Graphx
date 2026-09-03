#include <engine/physics/include/collisionsolver.hpp>
#include <engine/core/include/services.hpp>
#include <iostream>

bool ClipSegmentToLine(std::vector<ContactPoint>& points, const glm::vec3& normal, float offset) {
    std::vector<ContactPoint> result;
    
    float d1 = glm::dot(normal, points[0].position) - offset;
    float d2 = glm::dot(normal, points[1].position) - offset;

    if (d1 >= 0.0f)
        result.emplace_back(points[0]);
    if (d2 >= 0.0f)
        result.emplace_back(points[1]);

    if (d1 < 0.0f && d2 >= 0.0f) {
        float t = d1 / (d1 - d2);

        result.emplace_back(points[0].id, points[0].position + t * (points[1].position - points[0].position));
    } else if (d1 >= 0.0f && d2 < 0.0f) {
        float t = d1 / (d1 - d2);

        result.emplace_back(points[1].id, points[0].position + t * (points[1].position - points[0].position));
    }

    points = result;

    return points.size() >= 2;
}

Contact CircleCircleCheck(Object& A, Object& B) {
    glm::vec3 AtoB = B.transform.position - A.transform.position;
    float distance = glm::length(AtoB);
    float combinedradius = std::get<CircleData>(A.collider->shapedata).radius + std::get<CircleData>(B.collider->shapedata).radius;

    return {
        A,
        B,
        {
        .colliding = distance < combinedradius,
        .normal = glm::normalize(AtoB),
        //.penetrationDepth = (combinedradius - distance) * 0.5f
        }
    };
}

Contact PolygonPolygonCheck(Object& A, Object& B) {
    std::vector<glm::vec3> normalsA = A.collider->getNormals();
    std::vector<glm::vec3> normalsB = B.collider->getNormals();
    std::vector<std::vector<glm::vec3>> edgesA = A.collider->getEdges();
    std::vector<std::vector<glm::vec3>> edgesB = B.collider->getEdges();

    glm::vec3 smallestaxis;
    float minoverlap = FLT_MAX;
    Collider* referenceCollider;
    Collider* incidentCollider;
    std::vector<glm::vec3> referenceEdge;
    std::vector<glm::vec3> incidentEdge;
    size_t referenceIt;

    for (size_t i = 0; i < normalsA.size(); ++i) {
        Projection p1 = A.collider->project(normalsA[i]);
        Projection p2 = B.collider->project(normalsA[i]);
        float overlap = p1.getOverlap(p2);
        if (overlap == 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = normalsA[i];
            referenceCollider = &*A.collider;
            incidentCollider = &*B.collider;
            referenceIt = i;
        }
    }
    for (size_t i = 0; i < normalsB.size(); ++i) {
        Projection p1 = A.collider->project(normalsB[i]);
        Projection p2 = B.collider->project(normalsB[i]);
        float overlap = p1.getOverlap(p2);
        if (overlap == 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = normalsB[i];
            referenceCollider = &*B.collider;
            incidentCollider = &*A.collider;
            referenceIt = i;
        }
    }

    if (glm::dot(B.transform.position - A.transform.position, smallestaxis) < 0.0f) {
        smallestaxis = -smallestaxis;
        std::swap(referenceCollider, incidentCollider);
    }

    referenceEdge = referenceCollider->getReferenceFace(smallestaxis);
    incidentEdge = incidentCollider->getIncidentFace(smallestaxis);

    glm::vec3 refDirection = glm::normalize(referenceEdge[1] - referenceEdge[0]);
    float offset1 = glm::dot(refDirection, referenceEdge[0]);
    float offset2 = glm::dot(-refDirection, referenceEdge[1]);
    float refOffset = glm::dot(-smallestaxis, referenceEdge[0]);
    
    std::vector<ContactPoint> clippedpoints = {
        ContactPoint(ContactID(referenceIt, 0), incidentEdge[0]),
        ContactPoint(ContactID(referenceIt, 1), incidentEdge[1])
    };
    std::vector<ContactPoint> contactpoints;
    if (ClipSegmentToLine(clippedpoints, refDirection, offset1)
        && ClipSegmentToLine(clippedpoints, -refDirection, offset2))
            for (auto& point : clippedpoints) {
                point.penetrationDepth = glm::dot(-smallestaxis, point.position) - refOffset;
                if (point.penetrationDepth <= 0)
                    contactpoints.emplace_back(std::move(point));
            }
    
    for (auto& point : referenceEdge) {
        Services::Get<DebugRenderer>().AddPoint({
            .position = point,
            .color = {0.0f, 1.0f, 0.0f}
        });
    }

    for (auto& point : contactpoints) {
        Services::Get<DebugRenderer>().AddPoint({
            .position = point.position,
            .color = {1.0f, 0.0f, 0.0f}
        });
    }

    return {
        A,
        B,
        {
        .colliding = true,
        .normal = smallestaxis,
        .contactPoints = contactpoints
        }
    };
}

CollisionSolver::CollisionSolver() {
    Dispatch[ToIndex(ColliderType::Circle)][ToIndex(ColliderType::Circle)] = CircleCircleCheck;
    Dispatch[ToIndex(ColliderType::Polygon)][ToIndex(ColliderType::Polygon)] = PolygonPolygonCheck;
    //Dispatch[ToIndex(ColliderType::Polygon)][ToIndex(ColliderType::Circle)] = PolygonCircleCheck;
    /*Dispatch[ToIndex(ColliderType::Circle)][ToIndex(ColliderType::Polygon)] = [](RigidBody& A, RigidBody& B){
        Contact contact = RectangleCircleCheck(B, A);
        std::swap(contact.A, contact.B);
        contact.manifold.normal *= -1.0f;
        return contact;
    };*/
}

/*Contact PolygonCircleCheck(RigidBody& A, RigidBody& B) {
    glm::vec3 RectanglePosition = glm::vec3{0.0f};
    glm::vec3 RectangletoCircle = B.position - A.position;
    glm::vec3 ClosestPointOnRectangle = glm::clamp(RectangletoCircle, RectanglePosition-A.collider.rectangle.halfExtents, RectanglePosition+A.collider.rectangle.halfExtents);
    glm::vec3 CircletoClosestPoint = ClosestPointOnRectangle - RectangletoCircle;
    float CircletoClosestPointDistance = glm::length(CircletoClosestPoint);

    return {
        &A,
        &B,
        {
            .colliding = CircletoClosestPointDistance < B.collider.circle.radius,
            .normal = -glm::normalize(CircletoClosestPoint),
            .penetrationDepth = B.collider.circle.radius - CircletoClosestPointDistance
        }
    };
}*/

void CollisionSolver::PreStep(Contact& contact, float deltaTime) {
    //contact.K = contact.A.rigidbody->GetInverseMass() + contact.B.rigidbody->GetInverseMass();
}

void CollisionSolver::Resolve(Contact& contact) {
    if (!contact.manifold.colliding)
        return;

    for (size_t i = 0; i < contact.manifold.contactPoints.size(); ++i) {
        ContactPoint& contactpoint = contact.manifold.contactPoints[i];
        glm::vec3 relativevelocity = contact.B.rigidbody->GetPointVelocity(contactpoint.position) - contact.A.rigidbody->GetPointVelocity(contactpoint.position);
        float restitution = 0.0f;
        float vn = glm::dot(relativevelocity, contact.manifold.normal);

        if (vn > 0.0f)
            continue;
        
        if (vn < -10.0f) {
            restitution = 0.4f;
        }

        float ArmA = Cross2D(contactpoint.position - contact.A.rigidbody->position, contact.manifold.normal);
        float ArmB = Cross2D(contactpoint.position - contact.B.rigidbody->position, contact.manifold.normal);
        float K = 
            contact.A.rigidbody->GetInverseMass()
            + contact.B.rigidbody->GetInverseMass()
            + ArmA * ArmA * contact.A.rigidbody->GetInverseInertia()
            + ArmB * ArmB * contact.B.rigidbody->GetInverseInertia();

        float deltaj = -(1.0f + restitution) * vn / (K);
        float oldj = contactpoint.normalImpulse;
        float newj = std::max(0.0f, deltaj + oldj);
        deltaj = newj - oldj;

        glm::vec3 impulse = deltaj * contact.manifold.normal;
        contact.A.rigidbody->ApplyImpulseAtPosition(-impulse , contactpoint.position);
        contact.B.rigidbody->ApplyImpulseAtPosition(impulse, contactpoint.position);

        contactpoint.normalImpulse = newj;
    }
}