#pragma once

#include <deque>
#include <engine/include/scene/object.hpp>
#include <engine/include/rendering/uniformbuffer.hpp>
#include <engine/include/rendering/mesh.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct MaterialData {
    glm::vec4 Color;
};

struct ObjectData {
    glm::mat4 Transform;
};

class Renderer {
public:
    Renderer();
    void Draw(std::deque<Object>& objects);
    void Draw(const Object& object);
private:
    UniformBuffer m_materialbuffer;
    UniformBuffer m_objectbuffer;
};