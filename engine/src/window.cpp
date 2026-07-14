#include <engine/include/window.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

Window::Window(int width, int height, std::string title) {
    if (!glfwInit()) {
        DebugError("GLFW failed to initialise!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_handle) {
        glfwTerminate();
        DebugError("Window handle failed to create!");
    }

    glfwMakeContextCurrent(m_handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        DebugError("Failed to initialise GLAD");
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(m_handle, framebuffer_size_callback);
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_handle);
}

bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_handle);
}