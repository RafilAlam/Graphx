#include <scene/mesh_factory.hpp>
#include <renderer/vertex.hpp>

namespace graphx {

Mesh CreateMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    Buffer VBO(vertices, static_cast<int>(vertices.size() / 3), sizeof(Vertex));
    Buffer EBO(indices, static_cast<int>(indices.size()), sizeof(unsigned int));
    VertexArray VAO(VBO.GetHandle(), EBO.GetHandle());
    return Mesh{
        std::move(vertices),
        std::move(indices),
        std::move(VBO),
        std::move(EBO),
        std::move(VAO)
    };
}

}
