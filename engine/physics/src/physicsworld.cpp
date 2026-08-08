#include <engine/physics/include/physicsworld.hpp>
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

    // Apply Gravity & Integrate Velocities
    for (auto& object : objects) {
        if (!object.rigidbody)
            continue;

        object.rigidbody->ApplyForce({0, object.rigidbody->mass * gravity, 0});
        m_integrator->IntegrateVelocity(*object.rigidbody, deltaTime);
        object.rigidbody->ClearAccumulatedForce();
    }

    // Update Collider World Coords
    for (auto& object : objects) {
        if (!object.collider)
            continue;

        UpdateCollider(object);
    }

    // Collision Detection
    for (int i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (!objectA.collider)
            continue;

        for (int j = i + 1; j < objects.size(); ++j) {
            Object& objectB = objects[j];
            if (!objectB.collider)
                continue;

            Contact contact = m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB);
            if (contact.manifold.colliding) {
                contacts.emplace_back(std::move(contact));
                std::cout << "Collision Detected!" << '\n';
            }
        }
    }

    // Iterative Solving
    for (int k=0; k<8; ++k) {
        for (auto& contact : contacts) {
            m_collisionsolver.PreStep(contact, deltaTime);
            m_collisionsolver.Resolve(contact);
        }
    }

    // Integrate Positions
    for (auto& object: objects) {
        if (!object.rigidbody)
            continue;

        m_integrator->IntegratePosition(*object.rigidbody, deltaTime);
    }
}