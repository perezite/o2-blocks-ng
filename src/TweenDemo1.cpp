#include "TweenDemo1.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "TweenV2.h"
#include "TweenChain.h"

using namespace std;
using namespace sb;

namespace tweenDemo1
{
    using namespace tweenFunctions;
    
    void demo0()
    {
        using namespace v2;

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

    void update1(Window& window, Quad& quad, v2::Tween<Vector2f>& tween)
    {
        using namespace v2;

        if (Input::isTouchGoingDown(1)) {
            Vector2f touchPosition = Input::getTouchPosition(window);
            float duration = random(1, 3);
            tween = Tween<Vector2f>(quad.getPosition(), touchPosition, duration, getRandomTweenFunction1());
        }
    }

    void demo1()
    {
        using namespace v2;

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

    void demo200()
    {
        // using namespace v3

        Window window(400, 400);
        Quad quad;

        quad.setScale(40, 40);
        quad.setPosition(-100);

        // Tweener2f tweener = Tweener2f(quad.getPosition())
//              .to(100, 2, smoothstep)
//              .to(200, 1, bounceOut);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            // if (Input::isKeyGoingDown(KeyCode::Return)) {
            //      Tween2f lastTween = *last(tweener.getTweens());
            //      tweener = Tweener2f(quad.getPosition())
            //          .to(-200, lastTween.getDuration(), lastTween.getTweeningFunction());
            // }
            // 
            // quad.setPosition(tweener.getValue());
            window.draw(quad);
            window.display();
        }
    }

    void demo100()
    {
        // using namespace v3

        Window window(400, 400);
        Quad quad;
        Quad quad2;
        Quad quad3;

        quad.setScale(40, 40);
        quad.setPosition(-100);

        quad2.setScale(40, 40);
        quad2.setPosition(100);

        quad3.setScale(40, 40);
        quad3.setPosition(-50, 0);

        // Tweener2f tweener = Tweener2f(quad.getPosition())
        //              .to(Vector2f(50, 100), 2, smoothstep)
        //              .to(Vector2f(50, -100), 1, bounceOut);

        // Tween2f tween = Tween2f(quad2.getPosition())
        //              .to(Vector2f(-50, 100, 1, bounceOut))
        //              .to(Vector2f(-100, -50), 2, smoothstep);
        // Tweener2f tweener2(tween);


        // Tween2f tween2 = Tween2f(quad3.getPosition())
        //              .to(Vector2f(50, 0), 4, linear);
        // Stopwatch watch;

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            //quad.setPosition(tweener.getValue());
            //quad2.setPosition(tweener2.getValue());
            //quad3.setPosition(tween2.getValue(watch.getElapsedSeconds()));
            window.draw(quad);
            window.display();
        }
    }

    void run()
    {
        demo200();
        //demo100();
        //demo1();
        //demo0();
    }
}