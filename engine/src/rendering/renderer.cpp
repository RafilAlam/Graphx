#include <engine/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MaterialData), 0)
{}

void Renderer::Draw(Object object) {
    const Material& material = object.GetMaterial();
    UpdateData(material);
    glUseProgram(material.GetShader().GetProgram());
    glBindVertexArray(object.GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::Draw(std::deque<Object>& objects) {
    for (const auto& object : objects) {
        Draw(object);
    }
}

void Renderer::UpdateData(const Material& material) {
    m_materialbuffer.Upload(MaterialData(material.baseColor));
}