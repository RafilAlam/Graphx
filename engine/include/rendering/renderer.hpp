#pragma once

#include <deque>
#include <engine/include/scene/object.hpp>
#include <engine/include/rendering/mesh.hpp>

class Renderer {
public:
    void Draw(std::deque<Object>& objects);
    void Draw(Object object);
};