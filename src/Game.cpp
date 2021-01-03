#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Board.h"
#include <iostream>

using namespace sb;
using namespace std;

namespace configuration
{
    bool printFramerate = true;
}

namespace 
{
    void printFramerate()
    {
        if (!configuration::printFramerate)
            return;

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
}

namespace blocks
{
    struct BlocksGame
    {
        Window window;
        Board board;

        void start()
        {
            window.setSize(300, 540);
            window.setFramerateLimit(65);
            window.getCamera().requestSize(10, 18);
            window.getCamera().setCenter(.5f * window.getCamera().getSize());
            board.start();
        }

        void run()
        {
            start();

            while (window.isOpen()) {
                Input::update();
                window.update();
                board.update(window);
                window.clear(Color(1, 1, 1, 1));    
                board.draw(window);
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
