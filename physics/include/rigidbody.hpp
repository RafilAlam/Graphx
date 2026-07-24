#pragma once

#include <glm/glm.hpp>

class RigidBody {
public:
    RigidBody(float Mass);
    
    void ApplyImpulse(glm::vec3 impulsevector);
    void ApplyForce(glm::vec3 forcevector);
    glm::vec3 GetAccumulatedForce();
    float GetInverseMass();

    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    float mass{1.0f};

private:
    float m_inversemass{1.0f};
    glm::vec3 m_accumulatedforce{0.0f};
};