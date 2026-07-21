#include <engine/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "MyApp"
});

AssetManager assetmanager;

Mesh& triangle = assetmanager.LoadMesh("triangle", {
    Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    Vertex{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    Vertex{{0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.5f, 1.0f}},
});

Shader& baseshader = assetmanager.LoadShaders("baseshader", "assets/engine/shaders/default.vert", "assets/engine/shaders/default.frag");
Material& basematerial = assetmanager.LoadMaterial("basematerial", baseshader, Texture("assets/textures/container.jpg"));

Scene& scene = app.NewScene();
Object& mainobject = scene.CreateObject(triangle, basematerial);

int main() {
    app.Run();
}