#pragma once

#include <vector>
#include <unordered_map>

namespace graphx {

typedef unsigned int MeshHandle;

struct Mesh {
    std::vector<float> Vertices;
};

class AssetManager {
public:
    MeshHandle LoadMesh(std::vector<float> Vertices);
    Mesh GetMesh(MeshHandle Mesh);
private:
    std::unordered_map<MeshHandle, Mesh> meshes;
    unsigned int meshcount {0};
};

}