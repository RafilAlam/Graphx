#include <engine/physics/include/joint.hpp>

RodJoint::RodJoint(std::string name, Object& ObjectA, glm::vec3 localanchorA, Object& ObjectB, glm::vec3 localanchorB, float length): name(name), ObjectA(&ObjectA), localanchorA(localanchorA), ObjectB(&ObjectB), localanchorB(localanchorB), length(length) {}

void RodJoint::SolveJoint(float deltaTime) {
    RigidBody& BodyA = *ObjectA->rigidbody;
    RigidBody& BodyB = *ObjectB->rigidbody;

    glm::vec3 AnchorA = ObjectA->PointToWorldSpace(localanchorA);
    glm::vec3 AnchorB = ObjectB->PointToWorldSpace(localanchorB);

    glm::vec3 AtoB = AnchorB - AnchorA;
    glm::vec3 normal = glm::normalize(AtoB);
    float error = glm::length(AtoB) - length;

    float ArmA = Cross2D(AnchorA - BodyA.position, normal);
    float ArmB = Cross2D(AnchorB - BodyB.position, normal);
    float K = 
        BodyA.GetInverseMass()
        + BodyB.GetInverseMass()
        + ArmA * ArmA * BodyA.GetInverseInertia()
        + ArmB * ArmB * BodyB.GetInverseInertia();
    
    float vRelative = glm::dot((BodyB.GetPointVelocity(AnchorB) - BodyA.GetPointVelocity(AnchorA)), normal);

    float biasfactor = 1.0f;
    float vBias = biasfactor * -error / deltaTime;

    float lambda = (vBias - vRelative) / K;
    glm::vec3 impulse = lambda * normal;

    BodyA.ApplyImpulseAtPosition(-impulse, AnchorA);
    BodyB.ApplyImpulseAtPosition(impulse, AnchorB);
}