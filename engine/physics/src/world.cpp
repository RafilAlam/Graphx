#include <engine/physics/include/world.hpp>

void World::integratevelocity(RigidBody2D& rigidbody, float dt) {
    rigidbody.linearvelocity += rigidbody.m_forceaccumulator * rigidbody.mass * dt;
}
void World::integrateposition(RigidBody2D& rigidbody, float dt) {
    rigidbody.position += rigidbody.linearvelocity * dt;
}

void World::Simulate(float dt) {
    for (auto& rigidbody : rigidbodies) {
        rigidbody.ApplyForce(glm::vec2{0.0f, settings.gravity});
        integratevelocity(rigidbody, dt);
        integrateposition(rigidbody, dt);
    }
}