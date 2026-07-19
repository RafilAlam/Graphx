#pragma once

#include <engine/include/window.hpp>
#include <engine/include/debug.hpp>
#include <engine/include/scene/scene.hpp>
#include <engine/include/scene/object.hpp>
#include <engine/include/rendering/assetmanager.hpp>
#include <engine/include/rendering/renderer.hpp>
#include <engine/include/rendering/shader.hpp>
#include <engine/include/rendering/mesh.hpp>
#include <filesystem>
#include <vector>
#include <memory>

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
private:
    Window m_window;
    Renderer m_renderer;
    Scene m_scene;
    unsigned int m_shaderprogram;
};