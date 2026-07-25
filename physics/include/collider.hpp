#pragma once

#include <glm/glm.hpp>

enum class ColliderType {
    Rectangle,
    Circle,
    Count
};

constexpr int ToIndex(ColliderType type) {
    return static_cast<int>(type);
}

struct Collider {
    ColliderType type;

    union {
        struct {
            glm::vec3 halfExtents;
        } rectangle;

        struct {
            float radius;
        } circle;
    };
};