#include <engine/include/rendering/renderer.hpp>

void Renderer::Draw(Object object) {
    glUseProgram(object.GetShader().GetProgram());
    glBindVertexArray(object.GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::Draw(std::deque<Object>& objects) {
    for (const auto& object : objects) {
        Draw(object);
    }
}