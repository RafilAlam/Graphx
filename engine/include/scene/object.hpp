#pragma once

#include <engine/include/rendering/material.hpp>
#include <engine/include/rendering/mesh.hpp>
#include <engine/include/rendering/shader.hpp>
#include <glm/glm.hpp>

class Object {
public:
    Object(const Mesh& mesh, const Material& material);
    const Mesh& GetMesh();
    const Material& GetMaterial();
    glm::vec3 Position = {0.0f, 0.0f, 0.0f};
private:
    const Mesh* m_mesh;
    const Material* m_material;
};