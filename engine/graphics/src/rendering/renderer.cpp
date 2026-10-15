#include <engine/graphics/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MaterialData), 0),
   m_objectbuffer(sizeof(ObjectData), 1)
{}

void Renderer::Draw(const Mesh& mesh, const Material& material, const Transform& transform) {
    const Shader& shader = material.GetShader();

    glUseProgram(shader.GetProgram());

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, transform.position);
    modelmat *= glm::mat4_cast(transform.rotation);
    modelmat = glm::scale(modelmat, transform.scale);

    glm::mat4 viewmat = glm::mat4(1.0f);

    glm::mat4 projectionmat = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    m_objectbuffer.Upload(ObjectData{.Transform = projectionmat * viewmat * modelmat});
    m_materialbuffer.Upload(MaterialData{.Color = material.basecolor});

    material.texture.BindTexture();
    glBindVertexArray(mesh.GetVAO());

    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void Renderer::Draw(std::unordered_map<uint64_t, std::unique_ptr<Model>>& models) {
    for (const auto& modelpair : models) {
        const Model& model = *modelpair.second;
        Draw(model);
    }
}

void Renderer::Draw(const Model& model) {
    for (auto& objectpair : model.objects) {
        auto& object = objectpair.second;
        const Material& material = object->GetMaterial();
        const Shader& shader = material.GetShader();
        const StaticMesh& mesh = object->GetMesh();

        Draw(mesh, material, object->transform);
    }
}

void Renderer::Draw(const Scene& scene) {
    for (const auto& modelpair : scene.models) {
        const Model& model = *modelpair.second;
        Draw(model);
    }
    for (const auto& joint : scene.joints) {
        JointVisuals& visuals = *joint->visuals;
        Draw(visuals.mesh, visuals.material, visuals.transform);
    }
}