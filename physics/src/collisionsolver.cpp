#include <physics/include/collisionsolver.hpp>
#include <iostream>

CollisionSolver::CollisionSolver() {
    Dispatch[ToIndex(ColliderType::Circle)][ToIndex(ColliderType::Circle)] = CircleCircleCheck;
    Dispatch[ToIndex(ColliderType::Rectangle)][ToIndex(ColliderType::Circle)] = RectangleCircleCheck;
    Dispatch[ToIndex(ColliderType::Circle)][ToIndex(ColliderType::Rectangle)] = [](RigidBody& A, RigidBody& B){
        Contact contact = RectangleCircleCheck(B, A);
        std::swap(contact.A, contact.B);
        contact.manifold.normal *= -1.0f;
        return contact;
    };
}

Contact CircleCircleCheck(RigidBody& A, RigidBody& B) {
    glm::vec3 AtoB = B.position - A.position;
    float distance = glm::length(AtoB);
    float combinedradius = A.collider.circle.radius + B.collider.circle.radius;

    return {
        &A,
        &B,
        {
        .colliding = distance < combinedradius,
        .normal = glm::normalize(AtoB),
        .penetrationDepth = (combinedradius - distance) * 0.5f
        }
    };
}

Contact RectangleCircleCheck(RigidBody& A, RigidBody& B) {
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
}

void CollisionSolver::Resolve(Contact& contact) {
    glm::vec3 relativevelocity = contact.B->velocity - contact.A->velocity;
    if (!contact.manifold.colliding or glm::dot(relativevelocity, contact.manifold.normal) > 0)
        return;

    float impulse = (-1 * (1 + 1) * (glm::dot(relativevelocity, contact.manifold.normal))) / (contact.A->GetInverseMass() + contact.B->GetInverseMass());
    contact.A->ApplyImpulse(impulse * -contact.manifold.normal);
    contact.B->ApplyImpulse(impulse * contact.manifold.normal);
}