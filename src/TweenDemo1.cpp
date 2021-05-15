#include "TweenDemo1.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "TweenV2.h"
#include "TweenFunctions.h"

using namespace std;
using namespace sb;

namespace tweenDemo1
{
    void demo0()
    {
        using namespace tweenFunctions;

        Window window(400, 400);
        Quad quad;
        // Tweenf tween(100, smoothstep, 2).start();

        quad.setScale(40, 40);
        quad.setPosition(-100, 0);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            // quad.setPosition(tween.getValue());
            window.draw(quad);
            window.display();
        }
    }

    void run()
    {
        demo0();
    }
}