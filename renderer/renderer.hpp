#pragma once

#include <renderer/buffer.hpp>
#include <scene/scene.hpp>

namespace graphx {

struct Vertex {
    float pos[3];
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Render(Scene scene);
private:
    
};

}