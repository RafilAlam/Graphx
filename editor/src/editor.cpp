#include <editor/include/editor.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
});
AssetManager assetmanager;
Scene& scene = app.NewScene(assetmanager, "assets/demoscene.json");


int main() {
    app.Init();
    while (!app.shouldclose) {
        app.PhysicsStep();
        app.RenderStep();
    }
    app.Shutdown();
}