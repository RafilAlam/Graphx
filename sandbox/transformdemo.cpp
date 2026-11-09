#include <engine/core/include/app.hpp>
#include <engine/core/include/input.hpp>
#include <cmath>
#include <iostream>

App app({
    .WindowWidth = 500,
    .WindowHeight = 500,
    .WindowTitle = "TransformDemo"
}, "project");

//bool fPressed{false};

int main() {
    auto handle = Input::Subscribe([](Key key, KeyAction action, KeyModifier mod) {
        if (key == Key::F && action == KeyAction::Press) {
            Debug::Print("TransformDemo", "F Pressed!");
        }
        else if (key == Key::F && action == KeyAction::Release) {
            Debug::Print("TransformDemo", "F Released!");
        }
    });
    Input::Unsubscribe(handle);
    app.Run();
}