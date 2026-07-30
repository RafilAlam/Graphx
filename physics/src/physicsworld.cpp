#include <physics/include/physicsworld.hpp>
#include <iostream>

void PhysicsWorld::UpdateCollider(Object& object) {
    std::vector<glm::vec3>& localvertices = std::get<PolygonData>(object.collider->shapedata).localvertices;
    std::vector<glm::vec3>& worldvertices = std::get<PolygonData>(object.collider->shapedata).worldvertices;
    for (int i = 0; i < localvertices.size(); ++i) {
        // TODO: add rotation update, glm::rotate(glm::vec2(local.x, local.y), body.rotation);
        worldvertices[i] = (localvertices[i] * object.transform.scale + object.transform.position);
    }
}

void PhysicsWorld::Step(std::deque<Object>& objects, float deltaTime) {
    for (int i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (objectA.rigidbody)
            m_integrator->Integrate(*objectA.rigidbody, deltaTime);
        if (objectA.collider) {
            UpdateCollider(objectA);

            for (int j = i + 1; j < objects.size(); ++j) {
                Object& objectB = objects[j];
                Contact contact = m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB);
                m_collisionsolver.Resolve(contact);
                if (contact.manifold.colliding) {
                    std::cout << "Collision Detected!" << '\n';
                }
            }
        }
    }
}