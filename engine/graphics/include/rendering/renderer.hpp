#pragma once

#include <deque>
#include <engine/graphics/include/scene/scene.hpp>
#include <engine/graphics/include/scene/model.hpp>
#include <engine/graphics/include/rendering/uniformbuffer.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/core/include/debug.hpp>

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
    void Draw(const Mesh& mesh, const Material& material, const Transform& transform);
    void Draw(std::unordered_map<uint64_t, std::unique_ptr<Model>>& models);
    void Draw(const Model& model);
    void Draw(const Scene& scene);
private:
    UniformBuffer m_materialbuffer;
    UniformBuffer m_objectbuffer;
};