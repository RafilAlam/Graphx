#include <engine/include/app.hpp>

App::App(AppConfig config)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle))
 {}

int App::Run() {
    DebugPrint("Running!");
    while (!m_window.ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        m_window.SwapBuffers();
        glfwPollEvents();
    }

    DebugPrint("Closing...");
    glfwTerminate();
    DebugPrint("Closed.");
    return 0;
}