#include <engine/include/rendering/renderer.hpp>

void Renderer::Draw(Mesh mesh) {
    glBindVertexArray(mesh.GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}