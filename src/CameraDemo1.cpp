#include "CameraDemo1.h"
#include "Input.h"
#include "Window.h"

using namespace sb;

namespace cameraDemo1
{
    void demo99()
    {
        /*
            Window window(400, 800);
            Quad quad;
            
            window.requestCameraSize(4, 6);
            quad.setSize(3.9f, 7.9f);

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
        Window window;

        while (window.isOpen())
        {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.display();
        }
    }

    void run()
    {
        demo0();
    }
}
