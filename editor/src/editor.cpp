#include <editor/include/editor.hpp>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "GraphX Editor"
}, "project");


int main() {
    app.Run();
}