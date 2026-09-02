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

Scene& scene = app.NewScene(assetmanager, "assets/demoscene.scene");
Object& RectangleA = scene.objects[0];
Object& RectangleB = scene.objects[1];
Object& Ground = scene.objects[2];

class MainScript : public Script {
public:
    void OnStart() override {
        RectangleA.rigidbody->position = {200.0f, 50.0f, 5.0f};
        RectangleB.rigidbody->position = {-200.0f, 0.0f, 5.0f};
        Ground.rigidbody->position = {0.0f, -200.0f, 5.0f};
    }
    void OnUpdate() override {

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            RectangleB.rigidbody->ApplyImpulse({0.0f, 300.0f, 0.0f});
        }

        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
        }
    }
private:
    bool Fpressed{false};
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}