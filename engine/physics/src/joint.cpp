#include <engine/physics/include/joint.hpp>
/*
Joint::Joint(std::string name, Object* ObjectA, glm::vec3 localanchorA, Object* ObjectB, glm::vec3 localanchorB): name(name), ObjectA(ObjectA), localanchorA(localanchorA), ObjectB(ObjectB), localanchorB(localanchorB) {}

RodJoint::RodJoint(std::string name, Object* ObjectA, glm::vec3 localanchorA, Object* ObjectB, glm::vec3 localanchorB, float length, StaticMesh& mesh, Material& material): Joint(name, ObjectA, localanchorA, ObjectB, localanchorB), length(length) {
    visuals = std::make_unique<RodVisuals>(this, mesh, material);
}

void RodJoint::SolveJoint(float deltaTime) {
    RigidBody& BodyA = *ObjectA->rigidbody;
    RigidBody& BodyB = *ObjectB->rigidbody;

    anchorA = ObjectA->PointToWorldSpace(localanchorA);
    anchorB = ObjectB->PointToWorldSpace(localanchorB);

    glm::vec3 AtoB = anchorB - anchorA;
    glm::vec3 normal = glm::normalize(AtoB);
    float error = glm::length(AtoB) - length;

    float ArmA = Cross2D(anchorA - BodyA.position, normal);
    float ArmB = Cross2D(anchorB - BodyB.position, normal);
    float K = 
        BodyA.GetInverseMass()
        + BodyB.GetInverseMass()
        + ArmA * ArmA * BodyA.GetInverseInertia()
        + ArmB * ArmB * BodyB.GetInverseInertia();
    
    float vRelative = glm::dot((BodyB.GetPointVelocity(anchorB) - BodyA.GetPointVelocity(anchorA)), normal);

    float biasfactor = 1.0f;
    float vBias = biasfactor * -error / deltaTime;

    float lambda = (vBias - vRelative) / K;
    glm::vec3 impulse = lambda * normal;

    BodyA.ApplyImpulseAtPosition(-impulse, anchorA);
    BodyB.ApplyImpulseAtPosition(impulse, anchorB);
}

JointVisuals::JointVisuals(Joint* joint, StaticMesh& mesh, Material& material): joint(joint), mesh(mesh), material(material) {}

RodVisuals::RodVisuals(Joint* joint, StaticMesh& mesh, Material& material): JointVisuals(joint, mesh, material) {}

void RodVisuals::Update() {
    glm::vec3 direction = joint->anchorB - joint->anchorA;
    transform.position = (joint->anchorA + joint->anchorB) * 0.5f;
    float angle = glm::degrees(std::atan2(direction.y, direction.x));
    transform.SetRotation(glm::vec3(0.0f, 0.0f, 1.0f), angle);
    transform.scale = glm::vec3(glm::length(direction) * 0.5f, 5.0f, 1.0f);
}
*/