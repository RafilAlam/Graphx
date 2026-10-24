#pragma once

#include <deque>
#include <engine/graphics/include/project/project.hpp>
#include <engine/graphics/include/project/entity.hpp>
#include <engine/graphics/include/rendering/uniformbuffer.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/core/include/debug.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct MateriaBufferData {
    glm::vec4 Color;
};

struct ObjectBufferData {
    glm::mat4 Transform;
};

class Renderer {
public:
    Renderer();
    void Draw(AssetManager& assetmanager, const Mesh& mesh, const Material& material, const Transform& transform);
    void Draw(Scene& scene, AssetManager& assetmanager);
private:
    UniformBuffer m_materialbuffer;
    UniformBuffer m_objectbuffer;
};