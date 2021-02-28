#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Board.h"
#include "Backdrop.h"
#include "GameAssets.h"
#include <iostream>

using namespace sb;
using namespace std;

namespace configuration
{
    bool printFramerate = false;
}

namespace 
{
    void printFramerate()
    {
        static Stopwatch stopwatch;
        static size_t counter = 0;
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
        GameAssets gameAssets;
        Backdrop backdrop;
        Board board;

        BlocksGame()
            : board(gameAssets)
        { }

        void start()
        {
            window.setSize(300, 540);
            window.setFramerateLimit(65);
            window.getCamera().requestSize(10, 18);
            window.getCamera().setCenter(.5f * window.getCamera().getSize());
            board.start();
        }

        void update()
        {
            Input::update();
            window.update();
            backdrop.update(window.getCamera());
            board.updateCollider(Transform::Identity);
            board.update(window);
        }

        void draw()
        {
            window.draw(backdrop);
            window.draw(board);
        }

        void run()
        {
            start();

            while (window.isOpen()) {
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
