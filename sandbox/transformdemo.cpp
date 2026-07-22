#include <graphics/include/app.hpp>
#include <physics/include/integrator.hpp>
#include <cmath>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});

EulerIntegrator integrator;
AssetManager assetmanager;


RigidBody body{1.0f};
Mesh& plane = assetmanager.LoadMesh("assets/meshes/circle.obj");
Material& basematerial = assetmanager.LoadMaterial("assets/materials/base.mat");

Scene& scene = app.NewScene();
Object& mainobject = scene.CreateObject(plane, basematerial);

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        float dt = app.GetTime() - lastTime;

        if (!Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_PRESS) {
            Fpressed = true;
            body.ApplyImpulse({0.00001f, 0.0f, 0.0f});
        }

        if (Fpressed and app.GetInput(GLFW_KEY_F) == GLFW_RELEASE) {
            Fpressed = false;
        }

        integrator.Integrate(body, dt);
        mainobject.transform.position = body.position;
    }
private:
    float lastTime;
    bool Fpressed{false};
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}