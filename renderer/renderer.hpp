#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

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