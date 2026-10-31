#include <engine/core/include/app.hpp>
#include <cmath>
#include <iostream>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
}, "project");

//bool fPressed{false};

int main() {
    app.Run();
}