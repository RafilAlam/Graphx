#include <renderer/renderer.hpp>
#include <renderer/shader.hpp>
#include <assetmanager/assetmanager.hpp>

namespace graphx {

static const char* kVertexShader = R"(
#version 450 core
layout(location = 0) in vec3 pos;
void main() {
    gl_Position = vec4(pos, 1.0);
}
)";

static const char* kFragmentShader = R"(
#version 450 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

Renderer::Renderer() : shader(std::make_unique<Shader>(kVertexShader, kFragmentShader)) {}

Renderer::~Renderer() = default;

void Renderer::Render(flecs::world& world) {
    shader->Use();

    world.each([](flecs::entity, Mesh& mesh) {
        glBindVertexArray(mesh.VAO.GetHandle());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.Indices.size()), GL_UNSIGNED_INT, nullptr);
    });
}

}
