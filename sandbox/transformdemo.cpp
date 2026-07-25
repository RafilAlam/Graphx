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

Mesh& rectangle = assetmanager.LoadMesh("assets/meshes/plane.obj");
Mesh& circle = assetmanager.LoadMesh("assets/meshes/circle.obj");
Material& containermaterial = assetmanager.LoadMaterial("assets/materials/container.mat");
Material& concretematerial = assetmanager.LoadMaterial("assets/materials/concrete.mat");

Collider rectanglecollider{.type = ColliderType::Rectangle, .rectangle{.halfExtents = glm::vec3{100.0f, 100.0f, 0.0f}}};
Collider circlecollider{.type = ColliderType::Circle, .circle{.radius = 100.0f}};
RigidBody& bodyA = physicsworld.CreateRigidBody(1.0f, circlecollider);
RigidBody& bodyB = physicsworld.CreateRigidBody(1.0f, rectanglecollider);

Scene& scene = app.NewScene();
Object& objectA = scene.CreateObject(circle, containermaterial);
Object& objectB = scene.CreateObject(rectangle, concretematerial);

class MainScript : public Script {
public:
    void OnStart() override {
        bodyA.position = {-300.0f, 0.0f, 5.0f};
        bodyB.position = {300.0f, 0.0f, 5.0f};
    }
    void OnUpdate() override {
        float dt = app.GetTime() - lastTime;
        lastTime = app.GetTime();

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            bodyA.ApplyImpulse({100.0f, 0.0f, 0.0f});
            bodyB.ApplyImpulse({-100.0f, 0.0f, 0.0f});
        }

        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
        }

        physicsworld.Step(dt);
        objectA.transform.position = {bodyA.position.x, bodyA.position.y, 5.0f};
        objectA.transform.scale = {100.0f, 100.0f, 0.0f};

        objectB.transform.position = {bodyB.position.x, bodyB.position.y, 5.0f};
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