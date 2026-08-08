#include <engine/physics/include/integrator.hpp>

void EulerIntegrator::IntegrateVelocity(RigidBody& body, float deltaTime) {
    if (body.mass == 0)
        return;

    glm::vec3 acceleration = body.GetAccumulatedForce() * body.GetInverseMass();
    body.velocity += acceleration * deltaTime;
}

void EulerIntegrator::IntegratePosition(RigidBody& body, float deltaTime) {
    if (body.mass == 0)
        return;

    body.position += body.velocity * deltaTime;
}