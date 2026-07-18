#include <engine/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "MyApp"
});

AssetManager assetmanager;

Mesh& triangle = assetmanager.NewMesh({
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
});

Shader& baseshader = assetmanager.NewShader(ShaderConfig());

Object mainobject(triangle, baseshader);

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