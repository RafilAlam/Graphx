#include <core/engine.hpp>
using namespace graphx;

int main() {
    Window Window(800, 600);
    Application App(Window.GetHandle());

    Print("App Started!");
    App.Run();
}