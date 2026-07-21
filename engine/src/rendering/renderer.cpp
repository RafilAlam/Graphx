#include <engine/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MaterialData), 0),
   m_objectbuffer(sizeof(ObjectData), 1)
{}

void Renderer::Draw(Object object) {
    const Material& material = object.GetMaterial();

    m_materialbuffer.Upload(MaterialData{.Color = material.basecolor});

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, object.Position);
    m_objectbuffer.Upload(ObjectData{.Transform = transform});

    material.texture.BindTexture();

    glUseProgram(material.GetShader().GetProgram());
    glBindVertexArray(object.GetMesh().GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::Draw(std::deque<Object>& objects) {
    for (const auto& object : objects) {
        Draw(object);
    }
}