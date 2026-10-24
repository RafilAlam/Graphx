#include <engine/graphics/include/project/entity.hpp>

MeshRenderer::MeshRenderer(od::object& o)
: meshhandle(o["mesh"].get_uint32().value()), 
  materialhandle(o["material"].get_uint32().value()) 
{}

Transform::Transform(od::object& o)
: position(o["position"]["x"].get_float().value(),
           o["position"]["y"].get_float().value(),
           o["position"]["z"].get_float().value()),
  scale(o["half_extents"]["x"].get_float().value(), 
        o["half_extents"]["y"].get_float().value(), 
        o["half_extents"]["z"].get_float().value()) 
{}

void Transform::SetRotation(glm::vec3 degrees) {
    rotation = glm::quat(glm::radians(degrees));
}

void Transform::SetRotation(glm::vec3 axis, float degrees) {
    rotation = glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
}

void Transform::Rotate(glm::vec3 degrees) {
    glm::quat delta = glm::quat(glm::radians(degrees));
    rotation *= delta;
}

void Transform::Rotate(glm::vec3 axis, float degrees) {
    rotation *= glm::angleAxis(glm::radians(degrees), glm::normalize(axis));
}

glm::vec3 Transform::PointToWorldSpace(glm::vec3 localpoint) {
    return rotation * localpoint + position;
}