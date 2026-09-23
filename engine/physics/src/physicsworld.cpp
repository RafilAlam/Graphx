#include <engine/physics/include/physicsworld.hpp>
#include <engine/core/include/services.hpp>

void PhysicsWorld::UpdateCollider(Object& object) {
    std::vector<glm::vec3>& localvertices = std::get<PolygonData>(object.collider->shapedata).localvertices;
    std::vector<glm::vec3>& worldvertices = std::get<PolygonData>(object.collider->shapedata).worldvertices;
    glm::quat rotationQuat(object.rigidbody->rotation);
    glm::mat4 rotMatrix = glm::mat4_cast(rotationQuat);

    for (int i = 0; i < localvertices.size(); ++i) {
        glm::vec4 localposition(localvertices[i] * object.transform.scale, 1.0f);
        glm::vec3 rotatedPosition = glm::vec3(rotMatrix * localposition) + object.rigidbody->position;
        worldvertices[i] = rotatedPosition;
    }
}

void PhysicsWorld::Step(std::deque<Object>& objects, float deltaTime) {

    // Apply Gravity & Integrate Velocities
    for (auto& object : objects) {
        if (!object.rigidbody)
            continue;

        object.rigidbody->ApplyForce({0, object.rigidbody->GetMass() * gravity, 0});
        m_integrator->IntegrateVelocity(*object.rigidbody, deltaTime);
        object.rigidbody->ClearAccumulatedForce();
    }

    // Update Collider World Coords
    for (auto& object : objects) {
        if (object.collider)
            UpdateCollider(object);
    }

    // Collision Detection
    for (size_t i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (!objectA.collider)
            continue;

        for (size_t j = i + 1; j < objects.size(); ++j) {
            Object& objectB = objects[j];
            if (!objectB.collider)
                continue;
            Contact newContact = m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB);
            auto storedContact = contacts.find({&newContact.A, &newContact.B});
            if (storedContact != contacts.end()) {
                if (newContact.manifold.colliding) {
                    for (auto& newPoint : newContact.manifold.contactPoints) {
                        for (auto& storedPoint : storedContact->second.manifold.contactPoints) {
                            if (newPoint.id == storedPoint.id) {
                                newPoint.normalImpulse = storedPoint.normalImpulse;
                                break;
                            }
                        }
                    }
                    storedContact->second.manifold = std::move(newContact.manifold);
                } else {
                    contacts.erase(ShapePair(&newContact.A, &newContact.B));
                }
            } else if (newContact.manifold.colliding) {
                contacts.emplace(ShapePair(&newContact.A, &newContact.B), std::move(newContact));
            }
        }
    }

    // Iterative Solving
    for (int k=0; k<8; ++k) {
        for (auto& contact : contacts) {
            m_collisionsolver.Resolve(contact.second, deltaTime);
        }
    }

    // Integrate Positions
    for (auto& object: objects) {
        if (!object.rigidbody)
            continue;

        m_integrator->IntegrateTransform(*object.rigidbody, deltaTime);
    }
}