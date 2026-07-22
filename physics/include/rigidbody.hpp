#pragma once

#include <glm/glm.hpp>

class RigidBody {
public:
    RigidBody(float Mass);
    
    void ApplyImpulse(glm::vec3 impulsevector);

    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    float mass{1.0f};

private:
    float m_inversemass{1.0f};
};