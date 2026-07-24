#pragma once

#include <vector>
#include <graphics/include/rendering/mesh.hpp>

class Script {
public:
    virtual ~Script() = default;
    virtual void OnStart() {};
    virtual void OnUpdate() {};
};