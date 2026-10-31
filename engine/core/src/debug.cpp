#include <engine/core/include/debug.hpp>

void DebugError(std::string msg) {
    std::cerr << "GraphX ERROR! " << msg << '\n';
    std::exit(EXIT_FAILURE);
}