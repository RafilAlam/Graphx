#include <engine/physics/include/physicsworld.hpp>
#include <engine/core/include/services.hpp>

bool SameObjectPair(ContactID id, Contact contact) {
    return id.objectpair == ObjectPair(&contact.reference, &contact.incident);
}

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
    std::vector<Contact> oldContacts = std::move(m_contacts);
    m_contacts.clear();
    for (size_t i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (!objectA.collider)
            continue;

        for (size_t j = i + 1; j < objects.size(); ++j) {
            Object& objectB = objects[j];
            if (!objectB.collider)
                continue;
            Contact newContact = m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB);

            bool found = false;
            for (auto& newPoint : newContact.manifold.contactPoints) {
                for (auto& oldContact : oldContacts) {
                    if (!SameObjectPair(newPoint.id, oldContact))
                        continue;
                    for (auto& oldPoint : oldContact.manifold.contactPoints) {
                        if (newPoint.id == oldPoint.id) {
                            newPoint.normalImpulse = oldPoint.normalImpulse;
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        found = false;
                        break;
                    }
                }
            }
            if (newContact.manifold.colliding)
                m_contacts.emplace_back(std::move(newContact));
        }
    }

    // Iterative Solving
    for (auto& contact : m_contacts) {
        m_collisionsolver.WarmStart(contact);
    }
    for (int k=0; k<8; ++k) {
        for (auto& contact : m_contacts) {
            m_collisionsolver.Resolve(contact, deltaTime);
        }
    }
    for (auto& contact : m_contacts) {
        m_collisionsolver.PositionCorrection(contact, deltaTime);
    }

    // Integrate Positions
    for (auto& object: objects) {
        if (!object.rigidbody)
            continue;

        m_integrator->IntegrateTransform(*object.rigidbody, deltaTime);
    }
}