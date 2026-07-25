#include <physics/include/collisionsolver.hpp>
#include <iostream>

CollisionSolver::CollisionSolver() {
    Dispatch[ColliderType::Circle][ColliderType::Circle] = CircleCircleCheck;
}

Contact CircleCircleCheck(RigidBody& A, RigidBody& B) {
    glm::vec3 AtoB = B.position - A.position;
    float distance = glm::length(AtoB);
    float combinedradius = A.collider.circle.radius + B.collider.circle.radius;

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

void CollisionSolver::Resolve(Contact contact) {
    glm::vec3 relativevelocity = contact.B.velocity - contact.A.velocity;
    if (!contact.manifold.colliding or glm::dot(relativevelocity, contact.manifold.normal) > 0)
        return;

    float impulse = (-1 * (1 + 1) * (glm::dot(relativevelocity, contact.manifold.normal))) / (contact.A.GetInverseMass() + contact.B.GetInverseMass());
    contact.A.ApplyImpulse(impulse * -contact.manifold.normal);
    contact.B.ApplyImpulse(impulse * contact.manifold.normal);
}