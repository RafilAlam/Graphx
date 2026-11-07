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
    for (auto& transform : scene.transforms.dense) {
        MeshRenderer& meshrenderer = scene.meshrenderers[transform.handle.id];
        Mesh* mesh = assetmanager.GetMesh(meshrenderer.meshhandle);
        Material* material = assetmanager.GetMaterial(meshrenderer.materialhandle);
        if (mesh && material)
            Draw(assetmanager, *mesh, *material, transform);
        else {
            Debug::Error("Mesh and/or Material corrupt!");
        }
    }
}

void Renderer::draw_debug_points() {
    if (DebugDrawBuffer::m_pointvertices.empty())
        return;

    size_t numVertices = DebugDrawBuffer::m_pointvertices.size();
    glBindVertexArray(DebugDrawBuffer::m_arrays[0]);
    DebugDrawBuffer::FlushPoints();

    glDrawArrays(GL_POINTS, 0, numVertices);
}

void Renderer::draw_debug_lines() {
    if (DebugDrawBuffer::m_linevertices.empty())
        return;

    size_t numVertices = DebugDrawBuffer::m_linevertices.size();
    glBindVertexArray(DebugDrawBuffer::m_arrays[1]);
    DebugDrawBuffer::FlushLines();

    glDrawArrays(GL_LINES, 0, numVertices);
}

void Renderer::DrawDebug() {
    glUseProgram(DebugDrawBuffer::m_coreshader.GetProgram());

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::orthoLH(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);

    int transformLocation = glGetUniformLocation(DebugDrawBuffer::m_coreshader.GetProgram(), "Transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(projection * view));

    draw_debug_points();
    draw_debug_lines();
}