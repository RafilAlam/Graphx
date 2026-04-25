#include <renderer/renderer.hpp>

namespace graphx {

Renderer::Renderer() {

}

Renderer::~Renderer() {
    
}

void Renderer::Render(Scene scene) {
    for (EntityID i=0; i<scene.GetNumEntities(); ++i) {
        Mesh mesh = scene.Get<Mesh>(i);
    }
}

}