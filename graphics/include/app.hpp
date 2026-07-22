#pragma once

#include <graphics/include/window.hpp>
#include <graphics/include/debug.hpp>
#include <graphics/include/scene/scene.hpp>
#include <graphics/include/scene/object.hpp>
#include <graphics/include/rendering/assetmanager.hpp>
#include <graphics/include/rendering/renderer.hpp>
#include <graphics/include/rendering/shader.hpp>
#include <graphics/include/rendering/mesh.hpp>
#include <filesystem>
#include <vector>
#include <memory>
#include <chrono>

struct AppConfig {
    int WindowWidth = 100;
    int WindowHeight = 100;
    std::string WindowTitle = "GraphX";
};

class App {
public:
    App(AppConfig config);
    int Run();
    int GetInput(int keycode);
    Scene& NewScene();
    float GetTime();
private:
    Window m_window;
    Renderer m_renderer;
    Scene m_scene;
    unsigned int m_shaderprogram;
    std::chrono::steady_clock::time_point m_startTime;
};