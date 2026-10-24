#pragma once

#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/physics/include/rigidbody.hpp>
#include <simdjson.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace od = simdjson::ondemand;

struct Entity {
    uint32_t id;
};

struct MeshRenderer {
    MeshRenderer(od::object& o);
    AssetHandle meshhandle;
    AssetHandle materialhandle;
};

struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};

    Transform(od::object& o);
    void SetRotation(glm::vec3 degrees);
    void SetRotation(glm::vec3 axis, float degrees);
    void Rotate(glm::vec3 degrees);
    void Rotate(glm::vec3 axis, float degrees);
    glm::vec3 PointToWorldSpace(glm::vec3 localpoint);
};