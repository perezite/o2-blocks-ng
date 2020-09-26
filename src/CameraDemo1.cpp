#include "CameraDemo1.h"
#include "Input.h"
#include "Quad.h"
#include "Window.h"

using namespace sb;
using namespace std;

namespace cameraDemo1
{
    void demo99()
    {
        /*
            Window window(400, 800);
            Quad quad;
            
            window.getCamera().requestSize(4, 6);
            window.getCamera().setCenter(1, 1);
            quad.setSize(3.9f, 7.9f);
            quad.setPosition(1, 1);

            Vector2f actualCameraSize = window.getCamera().getActualSize();
            cout << actualCameraSize.x << " " << actualCameraSize.y << endl;

            while (window.isOpen())
            {
                Input::update();
                window.update();
                window.clear(Color(1, 1, 1, 1));
                window.draw(quad);
                window.display();
            }
        */
    }

    void demo0()
    {
        Window window(600, 400);
        Quad quad;

        quad.setScale(190);
        //window.getCamera().setWidth(400);
        window.getCamera().requestSize(200, 200);

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
        demo0();
    }
}
