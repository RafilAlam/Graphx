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

Mesh& circle = assetmanager.LoadMesh("assets/meshes/circle.obj");
Material& containermaterial = assetmanager.LoadMaterial("assets/materials/container.mat");
Material& concretematerial = assetmanager.LoadMaterial("assets/materials/concrete.mat");
Collider collider{.type = ColliderType::Circle, .circle{.radius = 100.0f}};
RigidBody& bodyA = physicsworld.CreateRigidBody(1.0f, collider);
RigidBody& bodyB = physicsworld.CreateRigidBody(1.0f, collider);

Scene& scene = app.NewScene();
Object& objectA = scene.CreateObject(circle, concretematerial);
Object& objectB = scene.CreateObject(circle, concretematerial);

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        float dt = app.GetTime() - lastTime;

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            bodyA.ApplyForce({0.001f, 0.0f, 0.0f});
        }

        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
        }

        bodyA.position = {0.0f, 0.0f, 5.0f};
        bodyB.position = {300.0f, 0.0f, 5.0f};

        physicsworld.Step(dt);
        objectA.transform.position = bodyA.position;
        objectA.transform.scale = {100.0f, 100.0f, 0.0f};

        objectB.transform.position = bodyB.position;
        objectB.transform.scale = {100.0f, 100.0f, 0.0f};
    }
private:
    float lastTime;
    bool Fpressed{false};
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}