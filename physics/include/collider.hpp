#pragma once

#include <glm/glm.hpp>

enum ColliderType {
    Rectangle,
    Circle,
    Count
};

struct Collider {
    ColliderType type;

    union {
        struct {
            glm::vec2 halfExtents;
        } rectangle;

        struct {
            float radius;
        } circle;
    };
};