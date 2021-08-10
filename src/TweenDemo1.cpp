#include "TweenDemo1.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "TweenDev.h"
#include "TweenChainDev.h"
#include "TweenerDev.h"

using namespace std;
using namespace sb;

namespace tweenDemo1
{
    using namespace v1::tweenFunctions;
    
    void demo0()
    {
        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);
        v1::Tween<float> tween(quad.getPosition().x, 100, 2, smoothstep);

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

    v1::TweenFunction getRandomTweenFunction1()
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

    void update1(Window& window, Quad& quad, v1::Tween<Vector2f>& tween)
    {
        if (Input::isTouchGoingDown(1)) {
            Vector2f touchPosition = Input::getTouchPosition(window);
            float duration = random(1, 3);
            tween = v1::Tween<Vector2f>(quad.getPosition(), touchPosition, duration, getRandomTweenFunction1());
        }
    }

    void demo1()
    {
        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);

        v1::Tween2f tween(quad.getPosition(), quad.getPosition());

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

    void demo100()
    {
        Window window(400, 400);
        Quad quad;
        Quad quad2;
        //Quad quad3;

        quad.setScale(10);
        quad.setPosition(-50);

        quad2.setScale(20);
        quad2.setPosition(-100);

        //quad3.setScale(30);
        //quad3.setPosition(-50, 50);

        v2::Tween2f tween = v2::Tween2f(quad.getPosition())
            .to(Vector2f(50, -50), 1, linear)
            .to(50, 3, smoothstep);
        Stopwatch watch;

        v2::Tween2f tween2 = v2::Tween2f(quad2.getPosition())
                    .to(Vector2f(100, -100), 1, smoothstep)
                    .to(100, 3, bounceOut);
        v1::Tweener2f tweener(tween2);

        // Tweener2f tweener2 = Tweener2f(quad.getPosition())
        //              .to(Vector2f(50, 100), 2, smoothstep)
        //              .to(Vector2f(50, -100), 1, bounceOut);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            quad.setPosition(tween.getValue(watch.getElapsedSeconds()));
            quad2.setPosition(tweener.getValue());
            //quad3.setPosition(tweener2.getValue());
            window.draw(quad);
            window.draw(quad2);
            //window.draw(quad3);
            window.display();
        }
    }

    void run()
    {
        demo100();
        //demo1();
        //demo0();
    }
}