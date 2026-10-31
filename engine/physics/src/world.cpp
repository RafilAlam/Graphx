#include <engine/physics/include/world.hpp>

void World::integratevelocity(RigidBody2D& rigidbody, float dt) {
    rigidbody.linearvelocity += rigidbody.m_forceaccumulator * rigidbody.properties.inversemass * dt;
    rigidbody.m_forceaccumulator = glm::vec2(0.0f);
}
void World::integrateposition(RigidBody2D& rigidbody, float dt) {
    rigidbody.position += rigidbody.linearvelocity * dt;
}

void World::Simulate(float dt) {
    for (auto& rigidbody : rigidbodies.dense) {
        integratevelocity(rigidbody, dt);
        integrateposition(rigidbody, dt);
        rigidbody.linearvelocity -= glm::vec2{0.0f, settings.gravity};
    }
}