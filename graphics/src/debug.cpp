#include <graphics/include/debug.hpp>

void DebugPrint(std::string msg) {
    std::cout << "GraphX | " << msg << '\n';
}

void DebugError(std::string msg) {
    std::cerr << "GraphX ERROR! " << msg << '\n';
    std::exit(EXIT_FAILURE);
}