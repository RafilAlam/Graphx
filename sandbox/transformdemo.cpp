#include <engine/core/include/app.hpp>
#include <engine/physics/include/physicsworld.hpp>
#include <cmath>
#include <iostream>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
}, "project");

//bool fPressed{false};

int main() {
    app.Init();
    while (!app.shouldclose) {
        //app.PhysicsStep();
        /*if (!fPressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            fPressed = true;
            //RectangleB.objects.at("Object")->rigidbody->ApplyImpulse({250.0f, 0.0f, 0.0f});
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
        }*/
        app.RenderStep();
    }
    app.Shutdown();
}