#pragma once

#include <iostream>
#include <string>

#include <dependencies/glad/glad.h>
#include <GLFW/glfw3.h>

namespace graphx {

class Window {
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;    

    Window(uint32_t width, uint32_t height);
    GLFWwindow* GetHandle();
private:
    GLFWwindow* handle;
};

}