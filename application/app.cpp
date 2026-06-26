#include <application/app.hpp>
#include <utilities/utils.hpp>
#include <scene/components.hpp>
#include <scene/mesh_factory.hpp>
#include <assetmanager/assetmanager.hpp>

#include <glfw/glfw3.h>

namespace graphx {

Application::Application(GLFWwindow* windowhandle) : window_handle(windowhandle) {
    world.component<Transform>();
    world.component<Details>();
    world.component<Mesh>();

    SetupDemoScene();
}

void Application::SetupDemoScene() {
    flecs::entity triangle = world.entity("Triangle");

    triangle.set<Transform>({});
    triangle.set<Details>({{1.0f, 0.5f, 0.2f, 1.0f}});

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    std::vector<unsigned int> indices = {0, 1, 2};

    triangle.set<Mesh>(CreateMesh(std::move(vertices), std::move(indices)));
}

void Application::Run() {
    Print("Application Started!");
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window_handle)) {
        float now = glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        OnUpdate(dt);
        OnRender();

        glfwSwapBuffers(window_handle);
    }

    Print("Application Stopped!");
    glfwTerminate();
}

void Application::OnUpdate(float /*dt*/) {}

void Application::OnRender() {
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer.Render(world);
}

}
