#include <engine/include/app.hpp>

App::App(AppConfig config)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle))
{}

int App::Run() {
    DebugPrint("Running!");
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_scene.OnStart();

    while (!m_window.ShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        m_renderer.Draw(m_scene.GetObjects());
        m_scene.Update();
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