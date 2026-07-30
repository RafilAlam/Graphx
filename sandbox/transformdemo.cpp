#include <graphics/include/app.hpp>
#include <physics/include/physicsworld.hpp>
#include <cmath>
#include <iostream>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});
AssetManager assetmanager;

Scene& scene = app.NewScene(assetmanager, "assets/demoscene.scene");
Object& Rectangle = scene.objects[0];
Object& Ball = scene.objects[1];

class MainScript : public Script {
public:
    void OnStart() override {
        Rectangle.rigidbody->position = {200.0f, 0.0f, 5.0f};
        Ball.rigidbody->position = {-200.0f, 0.0f, 5.0f};
        Ball.transform.scale = {100.0f, 100.0f, 0.0f};
        Rectangle.transform.scale = {100.0f, 100.0f, 0.0f};
    }
    void OnUpdate() override {

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            Ball.rigidbody->ApplyImpulse({100.0f, 0.0f, 0.0f});
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