#include <engine/include/app.hpp>
#include <cmath>

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

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        float sinvalue = std::sin(app.GetTime());
        float cosvalue = std::cos(app.GetTime());
        mainobject.transform.position = {0.1 * sinvalue, 0.0f, 0.0f};
        mainobject.transform.SetRotation({0.0f, 0.0f, 10.0f * sinvalue});
        mainobject.transform.scale = {0.1f * sinvalue + 1.0f, 0.2f * cosvalue + 1.0f, 0.0f};
    }
private:
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}