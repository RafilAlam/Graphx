#pragma once

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

#include <flecs.h>
#include <memory>

namespace graphx {

class Shader;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Render(flecs::world& world);
private:
    std::unique_ptr<Shader> shader;
};

}
