#include <assetmanager/assetmanager.hpp>
#include <utilities/utils.hpp>

namespace graphx {

MeshHandle AssetManager::LoadMesh(std::vector<float> Vertices) {
    MeshHandle handle = meshcount++;
    Mesh mesh = {Vertices};
    meshes[handle] = mesh;
}

Mesh AssetManager::GetMesh(MeshHandle handle) {
    auto mesh = meshes.find(handle);
    if (mesh != meshes.end()) Error("AssetManager failed to retrieve Mesh, " + handle);
    return mesh->second;
}

}