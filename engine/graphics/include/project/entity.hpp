#pragma once

#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/physics/include/rigidbody.hpp>
#include <yyjson.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct EntityHandle {
    uint32_t id;
};

struct MeshRenderer {
    MeshRenderer(AssetHandle meshhandle, AssetHandle materialhandle);
    AssetHandle meshhandle;
    AssetHandle materialhandle;
};

struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    Transform(yyjson_val* position, yyjson_val* half_extents);
    void SetRotation(glm::vec3 degrees);
    void SetRotation(glm::vec3 axis, float degrees);
    void Rotate(glm::vec3 degrees);
    void Rotate(glm::vec3 axis, float degrees);
    glm::vec3 PointToWorldSpace(glm::vec3 localpoint);
};