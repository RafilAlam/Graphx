#include <graphics/include/utils.hpp>

std::string LoadTextFile(std::string path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "File error: Could not open file at " << path << '\n';
        return "";
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string contents;
    contents.reserve(size);
    contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return contents;
}