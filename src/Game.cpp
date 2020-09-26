#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Backdrop.h"
#include "Quad.h"

using namespace sb;

namespace blocks
{
    void start(Window& window)
    {
        // window.requestCameraSize(10, 18);

        window.getCamera().setHeight(18);
        window.getCamera().setWidth(10);
    }

    void update(Window& window, Backdrop& backdrop)
    {
        backdrop.update(window.getCamera());
    }

    void draw(Window& window, Backdrop& backdrop, Quad& quad)
    {
        window.draw(backdrop);
        window.draw(quad);
    }

    void runGame()
    {
        Window window(400, 600);
        Backdrop backdrop;
        Quad quad;

        start(window);

        while (window.isOpen()) {
            Input::update();
            window.update();
            update(window, backdrop);
            window.clear(Color(1, 1, 1, 1));
            draw(window, backdrop, quad);
            window.display();
        }
    }
}
