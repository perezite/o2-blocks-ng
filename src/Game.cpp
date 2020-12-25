#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Backdrop.h"
#include "Quad.h"
#include "Logger.h"
#include "Block.h"
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
        Block block;

        void start()
        {
            window.setSize(300, 540);
            window.setFramerateLimit(65);
            window.getCamera().requestSize(10, 18);
            window.getCamera().setCenter(.5f * window.getCamera().getSize());
            block.setType('i');
        }

        void update()
        {
            backdrop.update(window.getCamera());
            //block.update();
        }

        void draw()
        {
            window.draw(backdrop);
            window.draw(block);
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
                SB_MESSAGE("fps: " << int(framerate));
            }
        }

        void printStats()
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
                printStats();
            }
        }
    };

    void runGame()
    {
        BlocksGame blocksGame;
        blocksGame.run();  
    }
}
