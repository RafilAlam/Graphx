#include <engine/graphics/include/rendering/assetmanager.hpp>

UUID::UUID(const char* uuid) {
    auto parsed = UUIDv4::UUID::fromStrFactory(uuid);
    parsed.bytes(data);
}

UUID::UUID(UUIDv4::UUID uuid) {
    uuid.bytes(data);
}

AssetRegistry::AssetRegistry() {
    CacheAssetPaths(std::filesystem::current_path() / "assets");
}

void AssetRegistry::CacheAssetPaths(std::filesystem::path assetsPath) {
    if (!std::filesystem::exists(assetsPath) || !std::filesystem::is_directory(assetsPath)) {
        DebugError("Invalid asset directory path.");
        return;
    }

    auto options = std::filesystem::directory_options::skip_permission_denied;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsPath, options)) {
        if (entry.is_regular_file()) {
            const auto& assetpath = entry.path();
            std::string assetext = assetpath.extension().string();
            if (!storeableExtensions.contains(assetext)) {
                DebugPrint("Ignored " + assetpath.string());
                continue;
            }
            auto metapath = assetpath;
            metapath.replace_extension(assetext + ".gxmeta");
            UUIDv4::UUID internaluuid;
            if (std::filesystem::exists(metapath)) {
                    DebugPrint("Fetching UUID for " + assetpath.string());
                    yyjson_doc* doc = yyjson_read_file(metapath.string().c_str(), 0, nullptr, nullptr);
                    if (!doc) {
                        DebugError("Failed to parse: " + metapath.string());
                        continue;
                    }

                    yyjson_val* root = yyjson_doc_get_root(doc);
                    yyjson_val* uuid = yyjson_obj_get(root, "uuid");

                    if (!yyjson_is_str(uuid)) {
                        DebugError("Missing or invalid UUID: " + metapath.string());
                        yyjson_doc_free(doc);
                        continue;
                    }

                    internaluuid = UUIDv4::UUID::fromStrFactory(yyjson_get_str(uuid));
                    
                    yyjson_doc_free(doc);
                    DebugPrint("Fetched");
            } else {
                DebugPrint("Generating .gxmeta for " + assetpath.string());
                internaluuid = m_uuidGenerator.getUUID();
                yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
                yyjson_mut_val *root = yyjson_mut_obj(doc);
                yyjson_mut_doc_set_root(doc, root);
                yyjson_mut_obj_add_str(doc, root, "uuid", internaluuid.str().c_str());
                yyjson_write_flag flg = YYJSON_WRITE_PRETTY;
                yyjson_write_err err;
                
                bool success = yyjson_mut_write_file(metapath.string().c_str(), doc, flg, NULL, &err);
                yyjson_mut_doc_free(doc);
                if (!success) {
                    std::cerr << "Failed to write JSON metafile. Error: " << err.msg << '\n';
                }
                DebugPrint("Generated");
            }
            DebugPrint("Loading " + assetpath.string());
            UUID uuid = UUID(internaluuid);
            if (assetext == ".png" || assetext == ".jpg")
                textureMap.emplace(uuid, assetpath);
            else if (assetext == ".obj") {
                meshMap.emplace(uuid, assetpath);
            }
            else if (assetext == ".gxmat")
                materialMap.emplace(uuid, assetpath);
            else if (assetext == ".vert") {
                auto fassetpath = assetpath;
                fassetpath.replace_extension(".frag");
                
                if (!std::filesystem::exists(fassetpath))
                    fassetpath.clear();

                shaderMap.emplace(uuid, ShaderSources(assetpath, fassetpath));
            }
            DebugPrint("Loaded");
        }
    }
}

AssetHandle AssetPool::LoadMesh(std::string filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || !scene->mMeshes)
        DebugError("Failed to load mesh file: " + filepath);

    aiMesh* mesh = scene->mMeshes[0];

    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<Face> faces(mesh->mNumFaces);

    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
        const aiVector3d& vertex = mesh->mVertices[i];
        if (!mesh->HasNormals())
            DebugError("Mesh has no normals. File: " + filepath);
        const aiVector3d& normal = mesh->mNormals[i];
        glm::vec2 uv;
        if (mesh->HasTextureCoords(0)) {
            uv.x = mesh->mTextureCoords[0][i].x;
            uv.y = mesh->mTextureCoords[0][i].y;
        } else {
            uv = {0.0f, 0.0f};
        }

        vertices[i] = Vertex{
            .Position = {vertex.x, vertex.y, vertex.z},
            .Normal   = {normal.x, normal.y, normal.z},
            .UV       = uv
        };
    }

    for (unsigned int i=0; i<mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        faces[i] = Face{
            .a = face.mIndices[0],
            .b = face.mIndices[1],
            .c = face.mIndices[2],
        };
    }

    std::string name = mesh->mName.C_Str();
    DebugPrint(std::string("Mesh created by Asset Manager: " + name));
    meshMap.emplace_back(vertices, faces);
    return AssetHandle{static_cast<uint32_t>(meshMap.size() - 1)};
}

AssetHandle AssetPool::LoadTexture(std::string filepath) {
    textureMap.emplace_back(filepath.c_str());
    return AssetHandle{static_cast<uint32_t>(textureMap.size() - 1)};
}

AssetHandle AssetPool::LoadShader(std::string vsourcepath, std::string fsourcepath) {
    shaderMap.emplace_back(vsourcepath, fsourcepath);
    return AssetHandle{static_cast<uint32_t>(shaderMap.size() - 1)};
}

MaterialData AssetPool::LoadMaterialData(std::string filepath) {
    yyjson_doc* doc = yyjson_read_file(filepath.c_str(), 0, nullptr, nullptr);
    yyjson_val* root = yyjson_doc_get_root(doc);

    const char* texture = yyjson_get_str(yyjson_obj_get(root, "texture"));
    const char* shader = yyjson_get_str(yyjson_obj_get(root, "shader"));

    return {
        UUIDv4::UUID::fromStrFactory(texture),
        UUIDv4::UUID::fromStrFactory(shader)
    };
}

AssetHandle AssetPool::LoadMaterial(AssetHandle shaderhandle, AssetHandle texturehandle) {
    materialMap.emplace_back(shaderhandle, texturehandle);
    return AssetHandle{static_cast<uint32_t>(materialMap.size() - 1)};
}

AssetHandle AssetManager::GetMesh(UUID uuid) {
    if (meshMap.contains(uuid))
        return meshMap.at(uuid);

    AssetHandle handle = pool.LoadMesh(registry.meshMap.at(uuid).string());
    meshMap.emplace(uuid, handle);
    return handle;
}

Mesh* AssetManager::GetMesh(AssetHandle handle) {
    if (handle.index >= pool.meshMap.size())
        return nullptr;
    
    return &pool.meshMap.at(handle.index);
}

//TODO: REPLACE REGISTRY LOOKUP BY REGISTRY FUNCTION
AssetHandle AssetManager::GetTexture(UUID uuid) {
    if (textureMap.contains(uuid))
        return textureMap.at(uuid);
    
    AssetHandle handle = pool.LoadTexture(registry.textureMap.at(uuid).string());
    textureMap.emplace(uuid, handle);
    return handle;
}

Texture* AssetManager::GetTexture(AssetHandle handle) {
    if (handle.index >= pool.textureMap.size())
        return nullptr;
    
    return &pool.textureMap.at(handle.index);
}

AssetHandle AssetManager::GetShader(UUID uuid) {
    if (shaderMap.contains(uuid))
        return shaderMap.at(uuid);
    
    ShaderSources sources = registry.shaderMap.at(uuid);

    AssetHandle handle = pool.LoadShader(sources.vsource.string(), sources.fsource.string());
    shaderMap.emplace(uuid, handle);
    return handle;
}

Shader* AssetManager::GetShader(AssetHandle handle) {
    if (handle.index >= pool.shaderMap.size())
        return nullptr;

    return &pool.shaderMap.at(handle.index);
}

AssetHandle AssetManager::GetMaterial(UUID uuid) {
    if (materialMap.contains(uuid))
        return materialMap.at(uuid);
    
    MaterialData data = pool.LoadMaterialData(registry.materialMap.at(uuid).string());
    AssetHandle handle = pool.LoadMaterial(GetShader(data.shader), GetTexture(data.texture));
    materialMap.emplace(uuid, handle);
    return handle;
}

Material* AssetManager::GetMaterial(AssetHandle handle) {
    if (handle.index >= pool.materialMap.size())
        return nullptr;
    
    return &pool.materialMap.at(handle.index);
}