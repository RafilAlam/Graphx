#include <engine/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MaterialData), 0),
   m_objectbuffer(sizeof(ObjectData), 1)
{}

void Renderer::Draw(const Object& object) {
    const Material& material = object.GetMaterial();
    const Shader& shader = material.GetShader();
    const Mesh& mesh = object.GetMesh();

    m_materialbuffer.Upload(MaterialData{.Color = material.basecolor});

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, object.transform.position);
    transform *= glm::mat4_cast(object.transform.rotation);
    transform = glm::scale(transform, object.transform.scale);
    m_objectbuffer.Upload(ObjectData{.Transform = transform});

    material.texture.BindTexture();

    glUseProgram(shader.GetProgram());
    glBindVertexArray(mesh.GetVAO());
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(std::deque<Object>& objects) {
    for (const auto& object : objects) {
        Draw(object);
    }
}