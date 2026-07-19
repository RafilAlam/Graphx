#include <engine/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "MyApp"
});

AssetManager assetmanager;

Mesh& triangle = assetmanager.LoadMesh("triangle", {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
});

Shader& baseshader = assetmanager.LoadShaders("baseshader", "assets/engine/shaders/default.vert", "assets/engine/shaders/default.frag");
Material& basematerial = assetmanager.LoadMaterial("basematerial", baseshader);
Object mainobject(triangle, basematerial);

Scene& scene = app.NewScene();

class MainScript : public Script {
public:
    void OnUpdate() override {
        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            DebugPrint("F Key Pressed!");
        }
        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
            DebugPrint("F Key Released!");
        }
    }
private:
    bool Fpressed = false;
};

int main() {
    scene.AddScript<MainScript>();
    scene.AddObject(mainobject);

    app.Run();
}