#include <core/window.hpp>
#include <utilities/utils.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

namespace graphx {

Window::Window(uint32_t width, uint32_t height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, "Graphx", NULL, NULL);
    if (handle == NULL) {
        glfwTerminate();
        Error("FAILED TO CREATE WINDOW!");
    }
    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        Error("FAILED TO INITIALISE GLAD!");
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
}

GLFWwindow* Window::GetHandle() {
    return handle;
}

}