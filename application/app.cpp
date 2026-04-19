#include <application/app.hpp>
#include <utilities/utils.hpp>
#include <glfw/glfw3.h>

namespace graphx {

    Application::Application(GLFWwindow* windowhandle) : window_handle(windowhandle) {}

    void Application::Run() {
        Print("Application Started!");
        float lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(window_handle)) {
            float now = glfwGetTime();
            float dt = now - lastTime;
            lastTime = now;

            glfwPollEvents();

            //OnUpdate();
            //OnRender();

            glfwSwapBuffers(window_handle);
        }

        Print("Application Stopped!");
        glfwTerminate();
    }

}