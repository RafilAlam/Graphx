#include <engine/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "MyApp"
});

Mesh triangle({
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
});

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
    Scene& scene = app.NewScene();

    scene.AddScript<MainScript>();

    app.Run();
}