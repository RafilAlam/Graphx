#pragma once

#include <engine/include/window.hpp>
#include <engine/include/debug.hpp>

struct AppConfig {
    int WindowWidth = 100;
    int WindowHeight = 100;
    std::string WindowTitle = "GraphX";
};

class App {
public:
    App(AppConfig config);
    int Run();
private:
    Window m_window;
};