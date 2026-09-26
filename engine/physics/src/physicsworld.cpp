#include <engine/physics/include/physicsworld.hpp>
#include <engine/core/include/services.hpp>

bool SameObjectPair(ContactID& id, Contact& contact) {
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

void PhysicsWorld::AddContactEdge(RigidBody& rigidbody, ContactEdge* contactedge) {
    contactedge->prev = nullptr;
    contactedge->next = nullptr;
    if (rigidbody.contactList){
        rigidbody.contactList->prev = contactedge;
        contactedge->next = rigidbody.contactList;
    }
    rigidbody.contactList = contactedge;
}

void PhysicsWorld::UpdateContactGraph(Contact& contact) {
    contact.referenceEdge = ContactEdge{&contact, &*contact.incident.rigidbody};
    contact.incidentEdge = ContactEdge{&contact, &*contact.reference.rigidbody};

    AddContactEdge(*contact.reference.rigidbody, &contact.referenceEdge);
    AddContactEdge(*contact.incident.rigidbody, &contact.incidentEdge);
}

void PhysicsWorld::CreateIsland(RigidBody& rigidbody) {
    Island island;
    std::stack<RigidBody*> stack;

    stack.push(&rigidbody);
    rigidbody.islanded = true;

    while(!stack.empty()) {
        RigidBody* body = stack.top();
        stack.pop();

        island.rigidbodies.push_back(body);

        for (ContactEdge* edge=body->contactList; edge!=nullptr; edge=edge->next) {
            Contact* contact = edge->contact;
            if (contact->islanded)
                continue;
            
            island.contacts.push_back(contact);
            contact->islanded = true;

            RigidBody* other = edge->other;
            if (!other->islanded && other->GetMass() != 0.0f) {
                stack.push(other);
                other->islanded = true;
            }
        }
    }

    island.sleeping = true;
    for (auto* body : island.rigidbodies) {
        if (!body->sleeping) {
            island.sleeping = false;
            break;
        }
    }

    if (!island.sleeping) {
        for (auto* body : island.rigidbodies)
            body->Wake();
    }

    m_islands.push_back(std::make_unique<Island>(std::move(island)));
}

void PhysicsWorld::Step(std::deque<Object>& objects, float deltaTime) {

    // Apply Gravity & Integrate Velocities
    for (auto& object : objects) {
        if (!object.rigidbody || object.rigidbody->sleeping)
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
    std::vector<std::unique_ptr<Contact>> oldContacts;
    std::swap(m_contacts, oldContacts);

    for (auto& object : objects) {
        if (!object.rigidbody)
            continue;

        object.rigidbody->islanded = false;
        object.rigidbody->contactList = nullptr;
    }

    for (size_t i = 0; i < objects.size(); ++i) {
        Object& objectA = objects[i];
        if (!objectA.collider)
            continue;

        for (size_t j = i + 1; j < objects.size(); ++j) {
            Object& objectB = objects[j];
            if (!objectB.collider)
                continue;
            Contact newContact = m_collisionsolver.Dispatch[ToIndex(objectA.collider->type)][ToIndex(objectB.collider->type)](objectA, objectB);
            if (!newContact.manifold.colliding)
                continue;

            bool found = false;
            for (auto& newPoint : newContact.manifold.contactPoints) {
                for (auto& oldContact : oldContacts) {
                    if (!SameObjectPair(newPoint.id, *oldContact))
                        continue;
                    for (auto& oldPoint : oldContact->manifold.contactPoints) {
                        if (newPoint.id == oldPoint.id) {
                            newPoint.normalImpulse = oldPoint.normalImpulse;
                            newPoint.tangentImpulse = oldPoint.tangentImpulse;
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
            m_contacts.emplace_back(std::make_unique<Contact>(std::move(newContact)));
            UpdateContactGraph(*m_contacts.back());
        }
    }

    // Constructing Islands
    m_islands.clear();
    for (auto& object : objects) {
        if (object.rigidbody->islanded || object.rigidbody->GetMass() == 0.0f)
            continue;
        CreateIsland(*object.rigidbody);
    }

    // Iterative Solving
    for (auto& island : m_islands) {
        if (!island->sleeping) {
            for (auto& contact : island->contacts)
               m_collisionsolver.WarmStart(*contact);
        }
    }
    for (int k=0; k<8; ++k) {
        for (auto& island : m_islands) {
            if (!island->sleeping) {
                for (auto& contact : island->contacts)
                    m_collisionsolver.Resolve(*contact, deltaTime);
            }
        }
    }
    for (auto& island : m_islands) {
        if (!island->sleeping) {
            for (auto& contact : island->contacts)
                m_collisionsolver.PositionCorrection(*contact, deltaTime);
        }
    }

    // Integrate Positions
    for (auto& island : m_islands) {
        if (!island->sleeping) {
            for (auto& rigidbody : island->rigidbodies)
                m_integrator->IntegrateTransform(*rigidbody, deltaTime);
        }
    }

    for (auto& island : m_islands) {
        bool canSleep = true;

        for (auto* body : island->rigidbodies) {
            body->UpdateRestTime(deltaTime);
            if (!body->IsSleepy())
                canSleep = false;
        }

        if (canSleep) {
            island->sleeping = true;
            for (auto* body : island->rigidbodies) {
                body->sleeping = true;
                body->linearvelocity = glm::vec3(0.0f);
                body->angularvelocity = glm::vec3(0.0f);
            }
        }
    }
}