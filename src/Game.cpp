#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Backdrop.h"
#include "Quad.h"
#include <iostream>

using namespace sb;
using namespace std;

namespace blocks
{
    Backdrop backdrop;
    Quad quad;

    void start(Window& window)
    {
        window.getCamera().requestSize(10, 18);
    }

    void update(Window& window)
    {
        backdrop.update(window.getCamera());
    }

    void draw(Window& window)
    {
        window.draw(backdrop);
        window.draw(quad);
    }

    void runGame()
    {
        Window window(400, 600);

        start(window);

        while (window.isOpen()) {
            Input::update();
            window.update();
            update(window);
            window.clear(Color(1, 1, 1, 1));
            draw(window);
            window.display();
        }
    }
}
