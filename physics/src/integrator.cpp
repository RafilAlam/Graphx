#include <physics/include/integrator.hpp>

void EulerIntegrator::Integrate(RigidBody& body, float deltaTime) {
    glm::vec3 acceleration = body.GetAccumulatedForce() * body.GetInverseMass();
    body.velocity += acceleration * deltaTime;
    body.position += body.velocity * deltaTime;
}