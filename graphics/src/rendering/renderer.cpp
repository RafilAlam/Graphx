#include <graphics/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MaterialData), 0),
   m_objectbuffer(sizeof(ObjectData), 1)
{}

void Renderer::Draw(const Object& object) {
    const Material& material = object.GetMaterial();
    const Shader& shader = material.GetShader();
    const Mesh& mesh = object.GetMesh();

    m_materialbuffer.Upload(MaterialData{.Color = material.basecolor});

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, object.transform.position);
    model *= glm::mat4_cast(object.transform.rotation);
    model = glm::scale(model, object.transform.scale);

    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 projection = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    m_objectbuffer.Upload(ObjectData{.Transform = projection * view * model});

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