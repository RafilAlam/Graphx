#include <renderer/renderer.hpp>

namespace graphx {

Renderer::Renderer() {

}

Renderer::~Renderer() {
    
}

void Renderer::Render(Scene scene) {
    for (EntityID i=0; i<scene.GetNumEntities(); ++i) {
        Mesh mesh = scene.Get<Mesh>(i);
        glBindVertexArray(mesh.VAO.GetHandle());
        glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, NULL);
    }
}

}