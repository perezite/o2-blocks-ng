#include "CameraDemo1.h"
#include "Input.h"
#include "Quad.h"
#include "Window.h"

using namespace sb;
using namespace std;

namespace cameraDemo1
{
    void demo0()
    {
        Window window(400, 600);
        Quad quad1;
        Quad quad2;

        quad1.setScale(1, 1);
        quad2.setScale(0.5f);
        quad2.setPosition(-4, -4);
        window.getCamera().requestSize(10, 10);
        window.getCamera().setCenter(-4, -4);

        Vector2f actualCameraSize = window.getCamera().getSize();
        cout << actualCameraSize.x << " " << actualCameraSize.y << endl;

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(quad1);
            window.draw(quad2);
            window.display();
        }
    }

    void demo1() 
    {
        Window window(400, 400);
        Quad quad;

        window.getCamera().requestSize(2, 1);

        Vector2f cameraSize = window.getCamera().getSize();
        cout << cameraSize.x << " " << cameraSize.y << endl;

        // Vector2f requestedCameraSize = window.getCamera().getRequestedSize();
        // cout << requestedCameraSize.x << " " << requestedCameraSize.y << endl;

        // window.setSize(400, 200);
        // cameraSize = window.getCamera().getSize();
        // cout << cameraSize.x << " " << cameraSize.y << endl;

        quad.setScale(1, 1);

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(quad);
            window.display();
        }
    }

    void run()
    {
        demo1();
        // demo0();
    }
}
