#pragma once

#include <engine/core/include/common.hpp>
#include <engine/core/include/utils.hpp>
#include <engine/graphics/include/rendering/material.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <engine/graphics/include/rendering/texture.hpp>
#include <engine/graphics/include/project/entity.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <memory>
#include <uuid_v4.h>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <yyjson.h>

struct UUID {
    char data[16]{};

    UUID() = default;
    UUID(const char* uuid);
    UUID(UUIDv4::UUID uuid);

    bool operator==(const UUID& other) const noexcept {
        return std::memcmp(data, other.data, 16) == 0;
    }
    bool operator!=(const UUID& other) const noexcept {
        return !(*this == other);
    }
};

template <>
struct std::hash<UUID> {
    size_t operator()(const UUID& uuid) const noexcept {
        size_t hash_val = 0;
        std::memcpy(&hash_val, uuid.data, sizeof(hash_val));
        return hash_val;
    }
};

struct ShaderSources {
    std::filesystem::path vsource;
    std::filesystem::path fsource;
};

struct MaterialData {
    UUID texture;
    UUID shader;
};

//TODO: REPLACE UNORDERED_MAP FOR FLATMAPS
class AssetRegistry {
public:
    AssetRegistry();
    void CacheAssetPaths(std::filesystem::path assetsPath);

    std::unordered_map<UUID, std::filesystem::path> meshMap;
    std::unordered_map<UUID, std::filesystem::path> textureMap;
    std::unordered_map<UUID, ShaderSources> shaderMap;
    std::unordered_map<UUID, std::filesystem::path> materialMap;
private:
    UUIDv4::UUIDGenerator<std::mt19937_64> m_uuidGenerator;
    
    const std::unordered_set<std::string> storeableExtensions {".png", ".jpg", ".obj", ".vert", ".gxmat"};
};

class AssetPool {
public:
    std::vector<StaticMesh> meshMap;
    std::vector<Texture> textureMap;
    std::vector<Shader> shaderMap;
    std::vector<Material> materialMap;

    AssetHandle LoadMesh(std::string filepath);
    AssetHandle LoadTexture(std::string filepath);
    AssetHandle LoadShader(std::string vsourcepath, std::string fsourcepath);
    MaterialData LoadMaterialData(std::string filepath);
    AssetHandle LoadMaterial(AssetHandle shaderhandle, AssetHandle texturehandle);
};

//TODO: REPLACE UNORDERED_MAP FOR FLATMAPS
class AssetManager {
public:
    AssetManager() = default;
    AssetHandle GetMesh(UUID uuid);
    Mesh* GetMesh(AssetHandle handle);
    AssetHandle GetTexture(UUID uuid);
    Texture* GetTexture(AssetHandle handle);
    AssetHandle GetShader(UUID uuid);
    Shader* GetShader(AssetHandle handle);
    AssetHandle GetMaterial(UUID uuid);
    Material* GetMaterial(AssetHandle handle);
private:
    AssetRegistry registry;
    AssetPool pool;

    std::unordered_map<UUID, AssetHandle> meshMap;
    std::unordered_map<UUID, AssetHandle> textureMap;
    std::unordered_map<UUID, AssetHandle> shaderMap;
    std::unordered_map<UUID, AssetHandle> materialMap;
};