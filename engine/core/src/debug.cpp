#include <engine/core/include/debug.hpp>

namespace Debug {

    void Error(std::string msg) {
        std::cerr << "GraphX ERROR! " << msg << '\n';
        std::exit(EXIT_FAILURE);
    }

}