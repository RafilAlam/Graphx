#pragma once

#include <iostream>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Debug {

    namespace detail {

        template <typename T>
        inline decltype(auto) to_string(T&& arg) {
            return std::forward<T>(arg);
        }
        inline std::string to_string(glm::vec3 vec) {
            return "{ x: " + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + ", z: " + std::to_string(vec.z) + " }";
        }
        inline std::string to_string(glm::vec2 vec) {
            return "{ x: " + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + " }";
        }

    }

    template<typename... Args>
    void Print(std::string_view identity, Args&&... args) {
        std::cout << "GraphX | " << identity << " | ";
        (std::cout << ... << detail::to_string(std::forward<Args>(args)));
        std::cout << '\n';
    }

    void Error(std::string msg);

}