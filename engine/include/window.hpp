#pragma once

#include <engine/include/debug.hpp>
#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Window {
public:
    Window(int width, int height, std::string title);
    void SwapBuffers();
    bool ShouldClose();
private:
    GLFWwindow* m_handle;
};