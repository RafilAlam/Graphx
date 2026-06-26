#pragma once

#include <glm/glm.hpp>

namespace graphx {

struct Transform {
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};
};

struct Details {
    glm::vec4 color = {0, 0, 0, 0};
};

}
