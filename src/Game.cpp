#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Backdrop.h"
#include "Quad.h"

using namespace sb;

namespace
{
    using namespace blocks;

    Backdrop backdrop;
    Window window(400, 600);

    Quad quad;
}

namespace blocks
{
    void start()
    {
        // window.requestCameraSize(10, 18);

        window.getCamera().setHeight(18);
        window.getCamera().setWidth(10);
    }

    void update()
    {
        backdrop.update(window.getCamera());
    }

    void draw()
    {
        window.draw(backdrop);
        window.draw(quad);
    }

    void runGame()
    {
        start();

        while (window.isOpen()) {
            Input::update();
            window.update();
            update();
            window.clear(Color(1, 1, 1, 1));
            draw();
            window.display();
        }
    }
}
