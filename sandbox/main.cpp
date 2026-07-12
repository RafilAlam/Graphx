#include <engine/include/app.hpp>

int main() {
    App app({
        .WindowWidth = 500,
        .WindowHeight = 500,
        .WindowTitle = "MyApp"
    });

    app.Run();
}