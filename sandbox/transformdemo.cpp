#include <graphics/include/app.hpp>
#include <physics/include/physicsworld.hpp>
#include <cmath>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});
PhysicsWorld physicsworld;
AssetManager assetmanager;

Mesh& plane = assetmanager.LoadMesh("assets/meshes/circle.obj");
Material& basematerial = assetmanager.LoadMaterial("assets/materials/base.mat");
RigidBody& body = physicsworld.CreateRigidBody(1.0f);

Scene& scene = app.NewScene();
Object& mainobject = scene.CreateObject(plane, basematerial);

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        float dt = app.GetTime() - lastTime;

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            body.ApplyForce({0.001f, 0.0f, 0.0f});
        }

        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
        }

        physicsworld.Step(dt);
        mainobject.transform.position = {body.position.x, body.position.y, 5.0f};
        mainobject.transform.scale = {100.0f, 100.0f, 0.0f};
    }
private:
    float lastTime;
    bool Fpressed{false};
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}