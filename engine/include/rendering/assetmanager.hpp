#pragma once

#include <engine/include/rendering/material.hpp>
#include <engine/include/rendering/shader.hpp>
#include <engine/include/rendering/mesh.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager() = default;
    Mesh& LoadMesh(std::string name, std::vector<float> vertices);
    Shader& LoadShaders(std::string name, std::string vertexsourcepath, std::string fragmentsourcepath);
    Material& LoadMaterial(std::string name, Shader& shader);
    Shader& LoadShaderPreset(std::string name);
private:
    std::unordered_map<std::string, Material> m_materials;
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Mesh> m_meshes;
};