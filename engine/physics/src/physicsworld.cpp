#include <engine/physics/include/physicsworld.hpp>
#include <engine/core/include/services.hpp>

bool SameObjectPair(ContactID& id, Contact& contact) {
    return id.objectpair == ObjectPair(&contact.reference, &contact.incident);
}

PhysicsWorld::PhysicsWorld(Scene& scene): m_scene(scene) {}

void PhysicsWorld::UpdateCollider(EntityHandle handle) {
    Collider& collider = m_scene.colliders.at(handle.id);
    RigidBody& rigidbody = m_scene.rigidbodies.at(handle.id);
    Transform& transform = m_scene.transforms.at(handle.id);
    std::vector<glm::vec3>& localvertices = std::get<PolygonData>(collider.shapedata).localvertices;
    std::vector<glm::vec3>& worldvertices = std::get<PolygonData>(collider.shapedata).worldvertices;
    glm::quat rotationQuat(rigidbody.rotation);
    glm::mat4 rotMatrix = glm::mat4_cast(rotationQuat);

    for (int i = 0; i < localvertices.size(); ++i) {
        glm::vec4 localposition(localvertices[i] * transform.scale, 1.0f);
        glm::vec3 rotatedPosition = glm::vec3(rotMatrix * localposition) + rigidbody.position;
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
    contact.referenceEdge.contact = &contact;
    contact.referenceEdge.other = &*contact.incident.rigidbody;

    contact.incidentEdge.contact = &contact;
    contact.incidentEdge.other = &*contact.reference.rigidbody;

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

void PhysicsWorld::Step(float deltaTime) {

    // Apply Gravity & Integrate Velocities
    for (size_t i = 0; i < m_scene.rigidbodies.size(); ++i) {
        RigidBody& rigidbody = m_scene.rigidbodies.at(i);
        rigidbody.islanded = false;
        rigidbody.contactList = nullptr;
        
        if (!rigidbody.sleeping) {
            rigidbody.ApplyForce({0, rigidbody.GetMass() * gravity, 0});
            m_integrator->IntegrateVelocity(rigidbody, deltaTime);
            rigidbody.ClearAccumulatedForce();
            
            UpdateCollider(EntityHandle{i});
        }
    }

    // Collision Detection
    std::vector<std::unique_ptr<Contact>> oldContacts;
    std::swap(m_contacts, oldContacts);

    for (auto i = 0; i < m_scene.colliders.size(); ++i) {
        Collider& colliderA = m_scene.colliders.at(i);
        for (auto j = i + 1; j < m_scene.colliders.size(); ++j) {
            Collider& colliderB = m_scene.colliders.at(j);
            Contact newContact = m_collisionsolver.Dispatch[static_cast<int>(colliderA.type)][static_cast<int>(colliderB.type)](EntityHandle{i}, EntityHandle{j});
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
    for (auto i = 0; i < m_scene.rigidbodies.size(); ++i) {
        RigidBody& rigidbody = m_scene.rigidbodies.at(i);
        if (rigidbody.islanded || rigidbody.GetMass() == 0.0f)
            continue;
        CreateIsland(rigidbody);
    }

    for (auto& island : m_islands) {
        if (!island->sleeping) {
            for (auto& contact : island->contacts)
               m_collisionsolver.PreSolve(*contact);
        }
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
        /*for (auto& joint : joints) {
            joint->SolveJoint(deltaTime);
            Services::Get<DebugRenderer>().AddPoint({
                .position = joint->anchorA,
                .color = {1.0f, 1.0f, 1.0f}
            });
            Services::Get<DebugRenderer>().AddPoint({
                .position = joint->anchorB,
                .color = {1.0f, 1.0f, 1.0f}
            });
        }*/
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