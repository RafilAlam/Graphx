#include <engine/include/app.hpp>
#include <cmath>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});

AssetManager assetmanager;

Mesh& triangle = assetmanager.LoadMesh("assets/meshes/plane.obj");
Material& basematerial = assetmanager.LoadMaterial("assets/materials/base.material");

Scene& scene = app.NewScene();
Object& mainobject = scene.CreateObject(triangle, basematerial);

class MainScript : public Script {
public:
    void OnStart() override {}
    void OnUpdate() override {
        float sinvalue = std::sin(app.GetTime());
        float cosvalue = std::cos(app.GetTime());
        mainobject.transform.position = {0.1 * sinvalue, 0.0f, 0.0f};
        mainobject.transform.SetRotation({0.0f, 0.0f, 10.0f * sinvalue});
        mainobject.transform.scale = {0.05f * sinvalue + 0.5f, 0.05f * cosvalue + 0.5f, 0.0f};
    }
private:
};

int main() {
    scene.AddScript<MainScript>();

    app.Run();
}