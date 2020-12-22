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
    namespace configuration
    {
        bool printFramerate = true;
    }

    struct BlocksGame
    {
        Window window;
        Backdrop backdrop;
        Quad quad;

        void start()
        {
            window.setSize(400, 600);
            window.setFramerateLimit(100);
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

        void printFramerate()
        {
            static Stopwatch stopwatch;
            static int counter = 0;
            counter++;

            if (counter == 100) {
                float elapsedMs = stopwatch.getElapsedMs();
                stopwatch.reset();
                float framerate = (1000.0f * counter) / elapsedMs;
                counter = 0;
                cout << "fps: " << int(framerate) << endl;
            }
        }

        void stats()
        {
            if (configuration::printFramerate)
                printFramerate();
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
                stats();
            }
        }
    };

    void runGame()
    {
        BlocksGame blocksGame;
        blocksGame.run();  
    }
}
