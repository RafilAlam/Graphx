#pragma once
#include <core/window.hpp>

namespace graphx {

class Application {
public:
    GLFWwindow* window_handle;

    Application(GLFWwindow* windowhandle);

    void Run();
    void OnUpdate();
    void OnRender();
};

}