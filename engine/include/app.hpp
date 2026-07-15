#pragma once

#include <engine/include/window.hpp>
#include <engine/include/debug.hpp>
#include <glm/glm.hpp>
#include <functional>
#include <concepts>
#include <vector>
#include <memory>

class Layer {
public:
    virtual ~Layer() = default;
    virtual void OnUpdate() = 0;
};

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
    template<typename T>
    void AddLayer() {
        m_layers.push_back(
            std::make_unique<T>()
        );
    }
private:
    Window m_window;
    std::vector<std::unique_ptr<Layer>> m_layers;
};