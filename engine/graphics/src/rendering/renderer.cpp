#include <engine/graphics/include/rendering/renderer.hpp>

Renderer::Renderer()
 : m_materialbuffer(sizeof(MateriaBufferData), 0),
   m_objectbuffer(sizeof(ObjectBufferData), 1)
{}

void Renderer::Draw(AssetManager& assetmanager, const Mesh& mesh, const Material& material, const Transform& transform) {
    const Shader& shader = *assetmanager.GetShader(material.shaderhandle);

    glUseProgram(shader.GetProgram());

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, transform.position);
    modelmat *= glm::mat4_cast(transform.rotation);
    modelmat = glm::scale(modelmat, transform.scale);

    glm::mat4 viewmat = glm::mat4(1.0f);

    glm::mat4 projectionmat = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    m_objectbuffer.Upload(ObjectBufferData{.Transform = projectionmat * viewmat * modelmat});
    m_materialbuffer.Upload(MateriaBufferData{.Color = material.basecolor});

    assetmanager.GetTexture(material.texturehandle)->BindTexture();
    glBindVertexArray(mesh.GetVAO());

    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void Renderer::Draw(Scene& scene, AssetManager& assetmanager) {
    for (uint32_t i = 0; i < scene.transforms.size(); ++i) {
        MeshRenderer& meshrenderer = scene.meshrenderers.at(i);
        Mesh* mesh = assetmanager.GetMesh(meshrenderer.meshhandle);
        Material* material = assetmanager.GetMaterial(meshrenderer.materialhandle);
        if (mesh && material)
            Draw(assetmanager, *mesh, *material, scene.transforms.at(i));
        else {
            DebugError("Mesh and/or Material corrupt!");
        }
    }
}