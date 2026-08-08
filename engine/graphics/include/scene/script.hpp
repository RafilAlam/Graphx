#pragma once

#include <vector>
#include <engine/graphics/include/rendering/mesh.hpp>

class Script {
public:
    virtual ~Script() = default;
    virtual void OnStart() {};
    virtual void OnUpdate() {};
};