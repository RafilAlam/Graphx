#include <engine/include/rendering/assetmanager.hpp>

Mesh& AssetManager::NewMesh(std::vector<float> vertices) {
    return m_meshes.emplace_back(vertices);
}

Shader& AssetManager::NewShader(ShaderConfig config) {
    return m_shaders.emplace_back(config);
}