#include <engine/core/include/app.hpp>
#include <engine/physics/include/physicsworld.hpp>
#include <cmath>
#include <iostream>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});
AssetManager assetmanager;

Scene& scene = app.NewScene(assetmanager, "assets/demoscene.json");
Model& RectangleA = scene.models.at("RectangleA");
Model& RectangleB = scene.models.at("RectangleB");
Model& Ground = scene.models.at("Ground");

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {

        if (!fPressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            fPressed = true;
            RectangleB.objects.at("Object").rigidbody->ApplyImpulse({100.0f, 0.0f, 0.0f});
        }

        if (!scene.simulatephysics and app.GetInput(GLFW_KEY_SPACE) == GLFW_PRESS) {
            scene.rendering = true;
            scene.simulatephysics = true;
        }

        if (fPressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            fPressed = false;
        }

        if (scene.simulatephysics and app.GetInput(GLFW_KEY_SPACE) == GLFW_RELEASE) {
            scene.rendering = false;
            scene.simulatephysics = false;
        }
    }
private:
    bool fPressed{false};
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}