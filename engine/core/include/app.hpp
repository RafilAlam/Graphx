#pragma once

#include <engine/core/include/window.hpp>
#include <engine/core/include/input.hpp>
#include <engine/core/include/debug.hpp>
#include <engine/graphics/include/project/project.hpp>
#include <engine/graphics/include/project/entity.hpp>
#include <engine/graphics/include/rendering/assetmanager.hpp>
#include <engine/graphics/include/rendering/renderer.hpp>
#include <engine/graphics/include/rendering/shader.hpp>
#include <engine/graphics/include/rendering/mesh.hpp>
#include <filesystem>
#include <vector>
#include <memory>
#include <chrono>

class App;

class Layer {
public:
    virtual ~Layer() = default;
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void OnUpdate() = 0;
};

class PhysicsLayer : public Layer {
    App& app;
public:
    PhysicsLayer(App& app);
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
};

class RenderingLayer : public Layer {
    App& app;
public:
    RenderingLayer(App& app);
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
};

class LayerStack {
    std::vector<std::unique_ptr<Layer>> m_layers;
public:
    LayerStack(size_t numLayers);
    template<typename T, typename... Args>
    void PushLayer(Args&&... args) {
        static_assert(std::is_base_of_v<Layer, T>, "Class must inherit from Layer");
        std::unique_ptr<Layer>& layer = m_layers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        layer->OnAttach();
    }
    void Clear();
    void Update();
};

struct AppConfig {
    int WindowWidth = 100;
    int WindowHeight = 100;
    std::string WindowTitle = "GraphX";
};

class App {
    Window m_window;
    Renderer m_renderer;
    AssetManager m_assetmanager;
    LayerStack m_layerstack;
    Project m_project;
    std::chrono::steady_clock::time_point m_startTime;
public:
    App(AppConfig config, std::string projectname);
    void Shutdown();
    void Run();
    int GetInput(int keycode);
    Project& LoadProject(std::string filepath);
    Project& GetProject();
    Renderer& GetRenderer();
    AssetManager& GetAssetManager();
    Window& GetWindow();
    float GetTime();
    float lastFrame{0.0f};
    float currentFrame{0.0f};
    float dt{0.0f};
    bool shouldclose{false};
};