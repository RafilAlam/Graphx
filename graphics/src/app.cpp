#include <graphics/include/app.hpp>

App::App(AppConfig config)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle)),
   m_startTime(std::chrono::steady_clock::now())
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

float App::GetTime() {
    using namespace std::chrono;
    return duration<float>(steady_clock::now() - m_startTime).count();
}