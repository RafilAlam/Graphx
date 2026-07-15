#include <engine/include/app.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "MyApp"
});

class MainLayer : public Layer {
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

    app.AddLayer<MainLayer>();

    app.Run();
}