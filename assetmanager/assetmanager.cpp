#include <assetmanager/assetmanager.hpp>
#include <scene/mesh_factory.hpp>

namespace graphx {

MeshHandle AssetManager::LoadMesh(std::vector<float> Vertices, std::vector<unsigned int> Indices) {
    MeshHandle handle = meshcount++;
    meshes[handle] = CreateMesh(std::move(Vertices), std::move(Indices));
    return handle;
}

const Mesh& AssetManager::GetMesh(MeshHandle handle) const {
    auto mesh = meshes.find(handle);
    if (mesh == meshes.end()) {
        Error("AssetManager failed to retrieve Mesh, " + std::to_string(handle));
    }
    return mesh->second;
}

}
