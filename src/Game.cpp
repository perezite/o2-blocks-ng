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
    struct BlocksGame
    {
        Window window;
        Backdrop backdrop;
        Quad quad;

        BlocksGame() : window(400, 600)
        { }

        void start()
        {
            // window.setSize(400, 600)
            window.getCamera().requestSize(10, 18);
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

        void run()
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
    };

    void runGame()
    {
        BlocksGame blocksGame;
        blocksGame.run();  
    }
}
