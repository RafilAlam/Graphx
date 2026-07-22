#include <physics/include/integrator.hpp>

void EulerIntegrator::Integrate(RigidBody& body, float deltaTime) {
    body.position += body.velocity * deltaTime;
}