#include <physics/include/collisionsolver.hpp>
#include <iostream>

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
        .penetrationDepth = (combinedradius - distance) * 0.5f
        }
    };
}

Contact PolygonPolygonCheck(Object& A, Object& B) {
    std::vector<glm::vec3> axesA = A.collider->getAxes();
    std::vector<glm::vec3> axesB = B.collider->getAxes();
    float minoverlap = FLT_MAX;
    glm::vec3 smallestaxis;
    for (int i = 0; i < axesA.size(); ++i) {
        Projection p1 = A.collider->project(axesA[i]);
        Projection p2 = B.collider->project(axesA[i]);
        float overlap = p1.getOverlap(p2);
        //std::cout << overlap << '\n';
        if (overlap == 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = axesA[i];
        }
    }
    for (int i = 0; i < axesB.size(); ++i) {
        Projection p1 = A.collider->project(axesB[i]);
        Projection p2 = B.collider->project(axesB[i]);
        float overlap = p1.getOverlap(p2);
        //std::cout << overlap << '\n';
        if (overlap == 0.0f) {
            return {
                A,
                B,
                {.colliding = false}
            };
        } else if (overlap < minoverlap) {
            minoverlap = overlap;
            smallestaxis = axesB[i];
        }
    }

    if (glm::dot(B.transform.position - A.transform.position, smallestaxis) < 0.0f) {
        smallestaxis = -smallestaxis;
    }

    return {
        A,
        B,
        {
        .colliding = true,
        .normal = smallestaxis,
        .penetrationDepth = minoverlap
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
    contact.K = contact.A.rigidbody->GetInverseMass() + contact.B.rigidbody->GetInverseMass();
}

// Soft Constraints Resolver
void CollisionSolver::Resolve(Contact& contact) {
    glm::vec3 relativevelocity = contact.B.rigidbody->velocity - contact.A.rigidbody->velocity;
    if (!contact.manifold.colliding) {
        std::cout << "Aborted" << '\n';
        return;
    }

    float restitution = 0.4f;
    float vn = glm::dot(relativevelocity, contact.manifold.normal);
    
    float deltaimpulse = (-1.0f * (1.0f + restitution) * vn) / (contact.K);
    std::cout << deltaimpulse << '\n';

    contact.A.rigidbody->ApplyImpulse(deltaimpulse * -contact.manifold.normal);
    contact.B.rigidbody->ApplyImpulse(deltaimpulse * contact.manifold.normal);
}