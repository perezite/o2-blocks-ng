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

    void run()
    {
        demo0();
    }
}
