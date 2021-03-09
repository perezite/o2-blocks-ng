#include "SceneDemo1.h"
#include "Drawable.h"
#include "Transformable.h"
#include "Sprite.h"
#include "DrawTarget.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"

using namespace std;
using namespace sb;

namespace sceneDemo1
{
    struct Assets {
        Texture greenBlock;

        Assets()
            : greenBlock("Textures/GreenBlock.png")
        { }

    };

    class MySprite1 : public Drawable, public Transformable
    {
        Sprite _sprite;

    public:
        MySprite1(Texture& texture)
            : _sprite(texture)
        { }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            states.transform *= getTransform();
            target.draw(_sprite, states);
        }
    };

    void demo1() {
        Window window;
        Assets assets;
        Sprite sprite(assets.greenBlock);
        MySprite1 mySprite1(assets.greenBlock);
        Quad quad;

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        mySprite1.setScale(200);

        while (window.isOpen()) {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(mySprite1);
            window.display();
        }
    }

    void run()
    {
        demo1();
    }
}
