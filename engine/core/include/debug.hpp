#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>

template<typename... Args>
void DebugPrint(std::string_view identity, Args&&... args) {
    std::cout << "GraphX | " << identity << " | ";
    (std::cout << ... << args) << '\n';
}

void DebugError(std::string msg);