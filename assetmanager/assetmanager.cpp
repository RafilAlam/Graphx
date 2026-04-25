#include <assetmanager/assetmanager.hpp>

namespace graphx {

MeshHandle AssetManager::LoadMesh(std::vector<float> Vertices, std::vector<unsigned int> Indices) {
    MeshHandle handle = meshcount++;
    Buffer VBO(Vertices, Vertices.size()/3.0f, sizeof(Vertex));
    Buffer EBO(Indices, Indices.size(), sizeof(unsigned int));
    VertexArray VAO(VBO.GetHandle(), EBO.GetHandle());
    Mesh mesh = {Vertices, Indices, VBO, EBO, VAO};
    meshes[handle] = mesh;
}

Mesh AssetManager::GetMesh(MeshHandle handle) {
    auto mesh = meshes.find(handle);
    if (mesh != meshes.end()) Error("AssetManager failed to retrieve Mesh, " + handle);
    return mesh->second;
}

}