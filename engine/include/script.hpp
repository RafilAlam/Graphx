#pragma once

#include <vector>
#include <engine/include/rendering/mesh.hpp>

class Script {
public:
    virtual ~Script() = default;
    virtual void OnUpdate() {};
    virtual void OnRender() {};
};