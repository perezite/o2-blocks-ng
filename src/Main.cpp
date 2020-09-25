#include "Window.h"
#include "Input.h"

using namespace sb;

int main() {
    Window window;

    while (window.isOpen()) {
        Input::update();
        window.update();
        window.clear(Color(1, 1, 1, 1));
        window.display();
    }
}
