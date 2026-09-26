#include <engine/physics/include/collisionsolver.hpp>

bool ClipSegmentToLine(std::vector<ContactPoint>& points, const glm::vec3& normal, glm::vec3 referencePoint, size_t referenceVertexIndex, size_t incidentFaceIndex) {
    std::vector<ContactPoint> result;

    float offset = glm::dot(normal, referencePoint);
    
    float d1 = glm::dot(normal, points[0].position) - offset;
    float d2 = glm::dot(normal, points[1].position) - offset;

    if (d1 <= 0.0f)
        result.emplace_back(points[0]);
    if (d2 <= 0.0f)
        result.emplace_back(points[1]);

    if ((d1 <= 0.0f && d2 > 0.0f) || (d1 > 0.0f && d2 <= 0.0f)) {
        float t = d1 / (d1 - d2);
        ContactID newContactID = points[0].id;
        newContactID.feature1 = Feature(referenceVertexIndex, FeatureType::Vertex);
        newContactID.feature2 = Feature(incidentFaceIndex, FeatureType::Face);

        result.emplace_back(newContactID, points[0].position + t * (points[1].position - points[0].position));
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

    glm::vec3 smallestaxis;
    float minoverlap = FLT_MAX;
    Object* referenceObject;
    Object* incidentObject;
    ContactID newContactID;
    std::vector<glm::vec3> referenceEdge;
    std::vector<glm::vec3> incidentEdge;

    for (size_t i = 0; i < normalsA.size(); ++i) {
        Projection p1 = A.collider->project(normalsA[i]);
        Projection p2 = B.collider->project(normalsA[i]);
        float overlap = p1.getOverlap(p2);
        if (overlap < 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = normalsA[i];
            referenceObject = &A;
            incidentObject = &B;
        }
    }
    for (size_t i = 0; i < normalsB.size(); ++i) {
        Projection p1 = A.collider->project(normalsB[i]);
        Projection p2 = B.collider->project(normalsB[i]);
        float overlap = p1.getOverlap(p2);
        if (overlap < 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = normalsB[i];
            referenceObject = &B;
            incidentObject = &A;
        }
    }

    // Canonicalisation
    if (std::less<const void*>{}(&A, &B)) {
        newContactID.objectpair.Object1 = &A;
        newContactID.objectpair.Object2 = &B;
    } else {
        newContactID.objectpair.Object1 = &B;
        newContactID.objectpair.Object2 = &A;
    }

    if (glm::dot(incidentObject->rigidbody->position - referenceObject->rigidbody->position, smallestaxis) < 0.0f) {
        smallestaxis = -smallestaxis;
        DebugPrint("SWAPPED A AND B!");
    }

    referenceEdge = referenceObject->collider->getMostAligningFace(smallestaxis);
    incidentEdge = incidentObject->collider->getMostAligningFace(-smallestaxis);

    newContactID.feature1 = Feature(referenceEdge[2].x, FeatureType::Face);

    glm::vec3 refDirection = glm::normalize(referenceEdge[1] - referenceEdge[0]);
    float refOffset = glm::dot(smallestaxis, referenceEdge[0]);
    
    std::vector<ContactPoint> clippedpoints = {
        ContactPoint(newContactID, incidentEdge[0]),
        ContactPoint(newContactID, incidentEdge[1])
    };
    clippedpoints[0].id.feature2 = Feature(incidentEdge[2].x, FeatureType::Vertex);
    clippedpoints[1].id.feature2 = Feature(incidentEdge[2].y, FeatureType::Vertex);

    std::vector<ContactPoint> contactpoints;
    if (ClipSegmentToLine(clippedpoints, refDirection, referenceEdge[1], referenceEdge[2].y, incidentEdge[2].x)
    && ClipSegmentToLine(clippedpoints, -refDirection, referenceEdge[0], referenceEdge[2].x, incidentEdge[2].x))
    {
        for (auto& point : clippedpoints) {
            point.penetrationDepth = glm::dot(smallestaxis, point.position) - refOffset;
            if (point.penetrationDepth <= 0)
                contactpoints.emplace_back(std::move(point));
        }
    }

    if (referenceObject == newContactID.objectpair.Object2) {
        for (auto& point : contactpoints) {
            std::swap(point.id.feature1, point.id.feature2);
        }
    }
    
    for (auto& point : referenceEdge) {
        Services::Get<DebugRenderer>().AddPoint({
            .position = point,
            .color = {0.0f, 1.0f, 0.0f}
        });
    }

    for (auto& point : incidentEdge) {
        Services::Get<DebugRenderer>().AddPoint({
            .position = point,
            .color = {0.0f, 0.65f, 0.0f}
        });
    }

    for (auto& point : contactpoints) {
        Services::Get<DebugRenderer>().AddPoint({
            .position = point.position,
            .color = {1.0f, 0.0f, 0.0f}
        });
    }

    return {
        *referenceObject,
        *incidentObject,
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

void CollisionSolver::PositionCorrection(Contact& contact, float dt) {
    float k = 15.0f;
    float c = 0.2f;

    for (auto& contactpoint : contact.manifold.contactPoints) {
        glm::vec3 rA = contactpoint.position - contact.reference.rigidbody->position;
        glm::vec3 rB = contactpoint.position - contact.incident.rigidbody->position;
        float InvMassA = contact.reference.rigidbody->GetInverseMass();
        float InvMassB = contact.incident.rigidbody->GetInverseMass();
        float InvInertiaA = contact.reference.rigidbody->GetInverseInertia();
        float InvInertiaB = contact.incident.rigidbody->GetInverseInertia();
        float InvMeff = InvMassA + InvMassB + pow(Cross2D(rA, contact.manifold.normal), 2) * InvInertiaA + pow(Cross2D(rB, contact.manifold.normal), 2) * InvInertiaB;
        glm::vec3 vA = contact.reference.rigidbody->GetPointVelocity(contactpoint.position);
        glm::vec3 vB = contact.incident.rigidbody->GetPointVelocity(contactpoint.position);
        float relativeVelocity = glm::dot(vB - vA, contact.manifold.normal);

        float correctionImpulse = std::max(0.0f, -k * dt * contactpoint.penetrationDepth - c * dt * relativeVelocity) / (1 + k * dt * dt * InvMeff + c * dt * InvMeff);
        glm::vec3 impulse = (correctionImpulse) * contact.manifold.normal;
        contact.reference.rigidbody->ApplyImpulseAtPosition(-impulse , contactpoint.position);
        contact.incident.rigidbody->ApplyImpulseAtPosition(impulse, contactpoint.position);
    }
}

void CollisionSolver::WarmStart(Contact& contact) {
    for (auto& contactpoint : contact.manifold.contactPoints) {
        float oldNormalImpulse = contactpoint.normalImpulse;
        float oldTangentImpulse = contactpoint.tangentImpulse;
        
        glm::vec3 tangent(contact.manifold.normal.y, -contact.manifold.normal.x, 0.0f);
        glm::vec3 warmImpulse = oldNormalImpulse * contact.manifold.normal + oldTangentImpulse * tangent;
        contact.reference.rigidbody->ApplyImpulseAtPosition(-warmImpulse , contactpoint.position);
        contact.incident.rigidbody->ApplyImpulseAtPosition(warmImpulse, contactpoint.position);
    }
}

void CollisionSolver::Resolve(Contact& contact, float deltaTime) {
    if (!contact.manifold.colliding)
        return;

    for (size_t i = 0; i < contact.manifold.contactPoints.size(); ++i) {
        ContactPoint& contactpoint = contact.manifold.contactPoints[i];
        float restitution = 0.0f;

        glm::vec3 warmedrelativevelocity = contact.incident.rigidbody->GetPointVelocity(contactpoint.position) - contact.reference.rigidbody->GetPointVelocity(contactpoint.position);
        float warmedvn = glm::dot(warmedrelativevelocity, contact.manifold.normal);

        /*if (vn < -0.2f) {
            restitution = 0.4f;
        }*/

        // Normal Collision Impulse
        float ArmA = Cross2D(contactpoint.position - contact.reference.rigidbody->position, contact.manifold.normal);
        float ArmB = Cross2D(contactpoint.position - contact.incident.rigidbody->position, contact.manifold.normal);
        float K = 
            contact.reference.rigidbody->GetInverseMass()
            + contact.incident.rigidbody->GetInverseMass()
            + ArmA * ArmA * contact.reference.rigidbody->GetInverseInertia()
            + ArmB * ArmB * contact.incident.rigidbody->GetInverseInertia();

        float lambda = -warmedvn / (K);
        float oldNormalImpulse = contactpoint.normalImpulse;
        float newNormalImpulse = std::max(0.0f, lambda + oldNormalImpulse);
        float deltaNormalImpulse = newNormalImpulse - oldNormalImpulse;

        // Tangential Friction
        glm::vec3 tangent(contact.manifold.normal.y, -contact.manifold.normal.x, 0.0f);
        float slidingspeed = glm::dot(warmedrelativevelocity, tangent);

        ArmA = Cross2D(contactpoint.position - contact.reference.rigidbody->position, tangent);
        ArmB = Cross2D(contactpoint.position - contact.incident.rigidbody->position, tangent);
        K = 
        contact.reference.rigidbody->GetInverseMass()
        + contact.incident.rigidbody->GetInverseMass()
        + ArmA * ArmA * contact.reference.rigidbody->GetInverseInertia()
        + ArmB * ArmB * contact.incident.rigidbody->GetInverseInertia();

        lambda = -slidingspeed / K;
        float maxFriction = 0.2f * newNormalImpulse;
        float oldTangentImpulse = contactpoint.tangentImpulse;
        float newTangentImpulse = glm::clamp(lambda + oldTangentImpulse, -maxFriction, maxFriction);
        float deltaTangentImpulse = newTangentImpulse - oldTangentImpulse;

        glm::vec3 impulse = deltaNormalImpulse * contact.manifold.normal + deltaTangentImpulse * tangent;
        contact.reference.rigidbody->ApplyImpulseAtPosition(-impulse , contactpoint.position);
        contact.incident.rigidbody->ApplyImpulseAtPosition(impulse, contactpoint.position);

        contactpoint.normalImpulse = newNormalImpulse;
        contactpoint.tangentImpulse = newTangentImpulse;
    }
}