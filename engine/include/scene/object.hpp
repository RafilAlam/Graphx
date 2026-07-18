#pragma once

#include <engine/include/rendering/mesh.hpp>
#include <engine/include/rendering/shader.hpp>

class Object {
public:
    Object(const Mesh& mesh, const Shader& shader);
    const Mesh& GetMesh();
    const Shader& GetShader();
private:
    const Mesh* m_mesh;
    const Shader* m_shader;
};