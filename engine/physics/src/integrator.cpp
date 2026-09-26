#include <engine/physics/include/integrator.hpp>

void EulerIntegrator::IntegrateVelocity(RigidBody& body, float deltaTime) {
    if (body.GetMass() == 0)
        return;

    glm::vec3 acceleration = body.GetAccumulatedForce() * body.GetInverseMass();
    body.linearvelocity += acceleration * deltaTime;
}

void EulerIntegrator::IntegrateTransform(RigidBody& body, float deltaTime) {
    if (body.GetMass() == 0 || body.sleeping)
        return;

    body.position += body.linearvelocity * deltaTime;
    body.rotation += body.angularvelocity * deltaTime;
}