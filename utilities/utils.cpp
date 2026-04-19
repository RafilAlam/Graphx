#include <utilities/utils.hpp>
#include <cstdlib>

void Print(std::string msg) {
    std::cout << "Graphx | " << msg << std::endl;
}

void Warn(std::string msg) {
    std::cerr << "Graphx WARNING | " << msg << std::endl;
}

void Error(std::string msg) {
    std::cerr << "Graphx ERROR | " << msg << std::endl;
    std::exit(EXIT_FAILURE);
}