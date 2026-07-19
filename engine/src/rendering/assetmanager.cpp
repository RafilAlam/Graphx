#include <engine/include/rendering/assetmanager.hpp>

Mesh& AssetManager::LoadMesh(std::string name, std::vector<float> vertices) {
    if (m_meshes.contains(name)) {
        return m_meshes.at(name);
    } else {
        auto [it, inserted] = m_meshes.try_emplace(name, Mesh(vertices));
        return it->second;
    }
}

Shader& AssetManager::LoadShaders(std::string name, std::string vertexsourcepath, std::string fragmentsourcepath) {
    if (m_shaders.contains(name)) {
        return m_shaders.at(name);
    } else {
        auto [it, inserted] = m_shaders.try_emplace(name, Shader(vertexsourcepath, fragmentsourcepath));
        return it->second;
    }
}

Material& AssetManager::LoadMaterial(std::string name, Shader& shader) {
    if (m_materials.contains(name)) {
        return m_materials.at(name);
    } else {
        auto [it, inserted] = m_materials.try_emplace(name, Material(shader));
        return it->second;
    }
}