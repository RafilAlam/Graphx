#include <engine/core/include/app.hpp>

PhysicsLayer::PhysicsLayer(App& app): app(app) {}

void PhysicsLayer::OnAttach() {
    app.lastFrame = app.GetTime();
    app.currentFrame;
}
void PhysicsLayer::OnDetach() {}
void PhysicsLayer::OnUpdate() {
    app.currentFrame = app.GetTime();
    float dt = app.currentFrame - app.lastFrame;
    app.lastFrame = app.currentFrame;
    app.GetProject().GetActiveScene().Update(dt);
}

RenderingLayer::RenderingLayer(App& app): app(app) {}
void RenderingLayer::OnAttach() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}
void RenderingLayer::OnDetach() {}
void RenderingLayer::OnUpdate() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (app.GetProject().GetActiveScene().rendering) {
        app.GetRenderer().Draw(app.GetProject().GetActiveScene(), app.GetAssetManager());
        //app.m_debugrenderer.Draw();
        app.GetWindow().SwapBuffers();
    }
    
    glfwPollEvents();
    app.shouldclose = app.GetWindow().ShouldClose();
}

LayerStack::LayerStack(size_t numLayers) {
    m_layers.reserve(numLayers);
}

void LayerStack::Clear() {
    for (auto& layer : m_layers)
        layer->OnDetach();
}

void LayerStack::Update() {
    for (auto& layer : m_layers)
        layer->OnUpdate();
}

App::App(AppConfig config, std::string projectname)
 : m_window(Window(config.WindowWidth, config.WindowHeight, config.WindowTitle)),
   m_startTime(std::chrono::steady_clock::now()),
   m_project(&m_assetmanager, projectname),
   m_layerstack(1)
{
    Services::Set<DebugRenderer>(m_debugrenderer);
    m_layerstack.PushLayer<PhysicsLayer>(*this);
    m_layerstack.PushLayer<RenderingLayer>(*this);
}

void App::Shutdown() {
    DebugPrint("Closing...");
    m_layerstack.Clear();
    DebugPrint("Closed.");
}

void App::Run() {
    DebugPrint("Running!");
    while(!shouldclose)
        m_layerstack.Update();
}

int App::GetInput(int keycode) {
    return glfwGetKey(m_window.GetHandle(), keycode);
}

Project& App::LoadProject(std::string projectname) {
    m_project = Project{&m_assetmanager, projectname};
    return m_project;
}

Project& App::GetProject() {
    return m_project;
}

Renderer& App::GetRenderer() {
    return m_renderer;
}

AssetManager& App::GetAssetManager() {
    return m_assetmanager;
}

Window& App::GetWindow() {
    return m_window;
}

float App::GetTime() {
    using namespace std::chrono;
    return duration<float>(steady_clock::now() - m_startTime).count();
}