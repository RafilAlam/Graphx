#pragma once

#include <renderer/buffer.hpp>
#include <renderer/vertexarray.hpp>
#include <utilities/utils.hpp>

#include <vector>
#include <unordered_map>

namespace graphx {

typedef unsigned int MeshHandle;

struct Mesh {
    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;
    Buffer VBO;
    Buffer EBO;
    VertexArray VAO;
};

class AssetManager {
public:
    MeshHandle LoadMesh(std::vector<float> Vertices, std::vector<unsigned int> Indices);
    Mesh GetMesh(MeshHandle Mesh);
private:
    std::unordered_map<MeshHandle, Mesh> meshes;
    unsigned int meshcount {0};
};

}