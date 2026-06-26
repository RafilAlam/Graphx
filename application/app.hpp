#pragma once

#include <core/window.hpp>
#include <renderer/renderer.hpp>

#include <flecs.h>

namespace graphx {

class Application {
public:
    Application(GLFWwindow* windowhandle);

    void Run();

private:
    void OnUpdate(float dt);
    void OnRender();
    void SetupDemoScene();

    GLFWwindow* window_handle;
    flecs::world world;
    Renderer renderer;
};

}
