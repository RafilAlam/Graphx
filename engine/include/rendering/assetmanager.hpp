#pragma once

#include <engine/include/rendering/shader.hpp>
#include <engine/include/rendering/mesh.hpp>
#include <memory>
#include <string>
#include <deque>

class AssetManager {
public:
    AssetManager() = default;
    Mesh& NewMesh(std::vector<float> vertices);
    Shader& NewShader(ShaderConfig config);
private:
    std::deque<Shader> m_shaders;
    std::deque<Mesh> m_meshes;
};