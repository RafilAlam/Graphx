#pragma once

#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <engine/core/include/utils.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager() = default;
    Mesh& LoadMesh(std::string name, std::vector<Vertex> vertices, std::vector<Face> faces);
    Mesh& LoadMesh(std::string filepath);
  
    Shader& LoadShaders(std::string name, std::string vertexsourcepath, std::string fragmentsourcepath);
  
    Material& LoadMaterial(std::string name, Shader& shader, Texture&& texture);
    Material& LoadMaterial(std::string filepath);
private:
    std::unordered_map<std::string, Material> m_materials;
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Mesh> m_meshes;
};