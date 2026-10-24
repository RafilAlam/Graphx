#include <engine/core/include/app.hpp>

App::App(AppConfig config, std::string projectname)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle)),
   m_startTime(std::chrono::steady_clock::now()),
   m_project(&m_assetmanager, projectname)
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
    m_project.GetActiveScene().Update(dt);
}

void App::RenderStep() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_project.GetActiveScene().rendering) {
        m_renderer.Draw(m_project.GetActiveScene(), m_assetmanager);
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

Project& App::LoadProject(std::string projectname) {
    m_project = Project{&m_assetmanager, projectname};
    return m_project;
}

float App::GetTime() {
    using namespace std::chrono;
    return duration<float>(steady_clock::now() - m_startTime).count();
}