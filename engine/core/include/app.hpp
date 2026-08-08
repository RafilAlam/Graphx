#pragma once

#include <engine/core/include/window.hpp>
#include <engine/core/include/services.hpp>
#include <engine/core/include/debug.hpp>
#include <engine/graphics/include/scene/scene.hpp>
#include <engine/graphics/include/scene/object.hpp>
#include <engine/graphics/include/rendering/assetmanager.hpp>
#include <engine/graphics/include/rendering/debugrenderer.hpp>
#include <engine/graphics/include/rendering/renderer.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
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
private:
    Window m_window;
    Renderer m_renderer;
    DebugRenderer m_debugrenderer;
    Scene m_scene;
    unsigned int m_shaderprogram;
    std::chrono::steady_clock::time_point m_startTime;
public:
    App(AppConfig config);
    int Run();
    int GetInput(int keycode);
    Scene& NewScene();
    Scene& NewScene(AssetManager& assetmanager, std::string filepath);
    float GetTime();
    float dt{0.0f};
};