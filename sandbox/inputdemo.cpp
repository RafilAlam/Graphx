#include <graphics/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "InputDemo"
});

AssetManager assetmanager;

Mesh& triangle = assetmanager.LoadMesh("triangle", 
{
    Vertex{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    Vertex{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    Vertex{{0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.5f, 1.0f}},
},
{
    Face{0, 1, 2}
});

Shader& baseshader = assetmanager.LoadShaders("baseshader", "assets/graphics/shaders/default.vert", "assets/graphics/shaders/default.frag");
Material& basematerial = assetmanager.LoadMaterial("basematerial", baseshader, Texture("assets/textures/container.jpg"));

Scene& scene = app.NewScene();
Object& mainobject = scene.CreateObject(triangle, basematerial);

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            DebugPrint("F Key Pressed!");
            mainobject.transform.Rotate({0.0f, 0.0f, 1.0f}, -45.0f);
        }
        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
            DebugPrint("F Key Released!");
            mainobject.transform.Rotate({0.0f, 0.0f, 1.0f}, 45.0f);
        }
    }
private:
    bool Fpressed = false;
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}