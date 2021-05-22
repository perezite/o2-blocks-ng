#include "TweenDemo1.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "TweenV2.h"

using namespace std;
using namespace sb;

namespace tweenDemo1
{
    using namespace v2;
    using namespace tweenFunctions;
    
    void demo0()
    {

        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);
        Tween<float> tween(quad.getPosition().x, 100, 2, smoothstep);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            quad.setPosition(tween.getValue());
            window.draw(quad);
            window.display();
        }
    }

    TweenFunction getRandomTweenFunction1()
    {
        int val = rand() % 3;
        if (val == 0) {
            cout << "linear" << endl;
            return linear;
        }
        else if (val == 1) {
            cout << "smoothstep" << endl;
            return smoothstep;
        }
        else {
            cout << "bounce out" << endl;
            return bounceOut;
        }
    }

    void update1(Window& window, Quad& quad, Tween<Vector2f>& tween)
    {
        if (Input::isTouchGoingDown(1)) {
            Vector2f touchPosition = Input::getTouchPosition(window);
            float duration = random(1, 3);
            tween = Tween<Vector2f>(quad.getPosition(), touchPosition, duration, getRandomTweenFunction1());
        }
    }

    void demo1()
    {
        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);

        Tween2f tween(quad.getPosition(), quad.getPosition());

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            update1(window, quad, tween);
            quad.setPosition(tween.getValue());
            window.draw(quad);
            window.display();
        }
    }

    void demo10()
    {
        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);

        // Tween2f tween(quad.getPosition(), quad.getPosition());

        // TweenChain2f tween(quad.getPosition())
        //      .to(Vector2f(100, -50), 2, smoothstep)
        //      .to(Vector2f(-50, 100), 3, bounceOut);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            //quad.setPosition(tween.getValue());
            window.draw(quad);
            window.display();
        }
    }

    void run()
    {
        demo1();
        //demo0();
    }
}