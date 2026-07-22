#pragma once

#include <graphics/include/rendering/material.hpp>
#include <graphics/include/rendering/shader.hpp>
#include <graphics/include/rendering/mesh.hpp>
#include <graphics/include/rendering/texture.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <graphics/include/utils.hpp>
#include <memory>
#include <string>
#include <deque>
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