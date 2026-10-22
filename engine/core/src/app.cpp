#include <engine/core/include/app.hpp>

App::App(AppConfig config)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle)),
   m_startTime(std::chrono::steady_clock::now())
{
    Services::Set<DebugRenderer>(m_debugrenderer);
}

void App::Init() {
    DebugPrint("Running!");
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    lastFrame = GetTime();
    currentFrame;
}

void App::PhysicsStep() {
    currentFrame = GetTime();
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    m_scene.Update(dt);
}

void App::RenderStep() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_scene.rendering) {
        m_renderer.Draw(m_scene);
        m_debugrenderer.Draw();
        m_window.SwapBuffers();
    }
    
    glfwPollEvents();
    shouldclose = m_window.ShouldClose();
}

void App::Shutdown() {
    DebugPrint("Closing...");
    glfwTerminate();
    DebugPrint("Closed.");
}

int App::GetInput(int keycode) {
    return glfwGetKey(m_window.GetHandle(), keycode);
}

Scene& App::NewScene(AssetManager& assetmanager) {
    m_scene = Scene{assetmanager};
    return m_scene;
}

Scene& App::NewScene(AssetManager& assetmanager, std::string filepath) {
    m_scene = Scene{assetmanager, filepath};
    return m_scene;
}

float App::GetTime() {
    using namespace std::chrono;
    return duration<float>(steady_clock::now() - m_startTime).count();
}