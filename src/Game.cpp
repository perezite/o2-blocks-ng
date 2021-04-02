#include "Game.h"
#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Board.h"
#include "Backdrop.h"
#include "Stats.h"
#include "GameAssets.h"
#include "VectorHelper.h"
#include <iostream>

using namespace sb;
using namespace std;

namespace blocks
{
    class Scene
    {
        Window window;
        GameAssets gameAssets;
        Backdrop backdrop;
        Board board;
        Vector2i size;

    public:
        Scene(size_t width, size_t height)
            : board(gameAssets, width, height),
              size(width, height)
        { }

        void start()
        {
            window.setSize(300, 540);
            window.setFramerateLimit(65);
            window.getCamera().requestSize(toVector2f(size));
            window.getCamera().setCenter(.5f * window.getCamera().getSize());
        }

        void update()
        {
            Input::update();
            window.update();
            board.updateColliders(Transform::Identity);
            backdrop.update(window.getCamera());
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
        //Scene scene(4, 4);
        Scene scene(10, 18);
        scene.run();
    }
}
