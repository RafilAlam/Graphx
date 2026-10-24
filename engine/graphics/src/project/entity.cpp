#include <engine/graphics/include/project/entity.hpp>

MeshRenderer::MeshRenderer(AssetHandle meshhandle, AssetHandle materialhandle)
: meshhandle(meshhandle), 
  materialhandle(materialhandle) 
{}

Transform::Transform(yyjson_val* position, yyjson_val* half_extents)
: position(static_cast<float>(yyjson_get_num(yyjson_obj_get(position, "x"))),
           static_cast<float>(yyjson_get_num(yyjson_obj_get(position, "y"))),
           static_cast<float>(yyjson_get_num(yyjson_obj_get(position, "z")))),
  scale(static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "x"))), 
        static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "y"))), 
        static_cast<float>(yyjson_get_num(yyjson_obj_get(half_extents, "z")))) 
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