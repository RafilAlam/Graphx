#include <engine/include/window.hpp>
#include <engine/include/debug.hpp>

Window::Window(int width, int height, std::string title) {
    if (!glfwInit()) {
        DebugError("GLFW failed to initialise!");
    }

    m_handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_handle) {
        glfwTerminate();
        DebugError("Window handle failed to create!");
    }

    glfwMakeContextCurrent(m_handle);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_handle);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_handle);
}