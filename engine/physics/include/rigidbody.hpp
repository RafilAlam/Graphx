#pragma once

#include <engine/physics/include/collider.hpp>
#include <engine/core/include/utils.hpp>
#include <glm/glm.hpp>

class RigidBody;
class Collider;
class Object;
struct Contact;

struct ObjectPair {
    Object* Object1;
    Object* Object2;

    bool operator==(const ObjectPair& other) const {
        return (Object1 == other.Object1 && Object2 == other.Object2) || (Object1 == other.Object2 && Object2 == other.Object1);
    }
};

enum class FeatureType {
    Vertex,
    Face
};

struct Feature {
    size_t index;
    FeatureType type;

    bool operator==(const Feature&) const = default;
};

struct ContactID {
    ObjectPair objectpair;
    Feature feature1;
    Feature feature2;

    bool operator==(const ContactID&) const = default;
};

struct ContactPoint {
    ContactID id;
    glm::vec3 position;
    float penetrationDepth;
    float normalImpulse;
    float tangentImpulse;
};

struct CollisionManifold {
    bool colliding{false};
    glm::vec3 normal{0.0f};
    std::vector<ContactPoint> contactPoints;
};

struct ContactEdge {
    Contact* contact;
    RigidBody* other;
    ContactEdge* next;
    ContactEdge* prev;
};

struct Contact {    
    Object& reference;
    Object& incident;
    
    CollisionManifold manifold;
    
    ContactEdge referenceEdge;
    ContactEdge incidentEdge;
    bool islanded{false};
};

class RigidBody {
public:
    RigidBody(float mass, glm::vec3 size);
    
    void ApplyImpulse(glm::vec3 impulse);
    void ApplyImpulseAtPosition(glm::vec3 impulse, glm::vec3 position);
    glm::vec3 GetPointVelocity(glm::vec3 point);

    void ApplyForce(glm::vec3 forcevector);
    glm::vec3 GetAccumulatedForce();
    void ClearAccumulatedForce();
    
    void SetSize(glm::vec3 size);
    void SetMass(float mass);

    float GetMass();
    float GetInverseMass();

    float GetInertia();
    float GetInverseInertia();

    bool IsResting();
    bool IsSleepy();
    void Wake();
    void UpdateRestTime(float dt);

    glm::vec3 position{0.0f};
    glm::vec3 linearvelocity{0.0f};

    glm::vec3 rotation{0.0f};
    glm::vec3 angularvelocity{0.0f};

    ContactEdge* contactList;
    bool islanded{false};

    float restTime{0.0f};
    bool sleeping{false};

private:
    glm::vec3 m_size{1.0f};    
    float m_mass{1.0f};
    float m_inversemass{1.0f};
    float m_inertia;
    float m_inverseinertia{1.0f};
    void m_recalculateinertia();
    glm::vec3 m_accumulatedforce{0.0f};
};