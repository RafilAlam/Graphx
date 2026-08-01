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
    std::vector<Contact> contacts;
    for (int i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (objectA.rigidbody) {
            objectA.rigidbody->ApplyForce({0, objectA.rigidbody->mass * gravity, 0});
            m_integrator->Integrate(*objectA.rigidbody, deltaTime);
        }
        if (objectA.collider) {
            UpdateCollider(objectA);

            for (int j = i + 1; j < objects.size(); ++j) {
                Object& objectB = objects[j];
                contacts.emplace_back(m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB));
                if (contacts.back().manifold.colliding) {
                    std::cout << "Collision Detected!" << '\n';
                }
            }
        }
    }
    for (int k=0; k<8; ++k) {
        for (auto& contact : contacts)
            m_collisionsolver.Resolve(contact);
    }
}