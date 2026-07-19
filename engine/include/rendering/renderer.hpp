#pragma once

#include <deque>
#include <engine/include/scene/object.hpp>
#include <engine/include/rendering/uniformbuffer.hpp>
#include <engine/include/rendering/mesh.hpp>
#include <glm/glm.hpp>

struct MaterialData {
    glm::vec4 Color;
};

class Renderer {
public:
    Renderer();
    void Draw(std::deque<Object>& objects);
    void Draw(Object object);
private:
    void UpdateData(const Material& material);
    UniformBuffer m_materialbuffer;
};