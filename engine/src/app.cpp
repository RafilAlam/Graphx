#include <engine/include/app.hpp>

App::App(AppConfig config)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle))
 {}

int App::Run() {
    DebugPrint("Running!");

    while (!m_window.ShouldClose()) {
        m_scene.Update();
        glClear(GL_COLOR_BUFFER_BIT);
        m_window.SwapBuffers();
        glfwPollEvents();
    }

    DebugPrint("Closing...");
    glfwTerminate();
    DebugPrint("Closed.");
    return 0;
}

int App::GetInput(int keycode) {
    return glfwGetKey(m_window.GetHandle(), keycode);
}

Scene& App::NewScene() {
    m_scene = Scene{};
    return m_scene;
}