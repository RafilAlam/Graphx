#pragma once

#include <engine/physics/include/collider.hpp>
#include <engine/core/include/utils.hpp>
#include <glm/glm.hpp>

class Collider;

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

    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};

    glm::vec3 rotation{0.0f};
    glm::vec3 angularvelocity{0.0f};

private:
    glm::vec3 m_size{1.0f};    
    float m_mass{1.0f};
    float m_inversemass{1.0f};
    float m_inertia;
    float m_inverseinertia{1.0f};
    void m_recalculateinertia();
    glm::vec3 m_accumulatedforce{0.0f};
};