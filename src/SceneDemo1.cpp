#include "SceneDemo1.h"
#include "Drawable.h"
#include "Transformable.h"
#include "Sprite.h"
#include "DrawTarget.h"
#include "Window.h"
#include "Input.h"
#include "Quad.h"
#include "Memory.h"
#include <vector>

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

    void demo100() {
        Window window;
        Assets assets;
        // Scene scene;

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        // scene.createDrawable<Sprite>(assets.greenBlock);
        // scene.create<MyEntity>(assets.greenBlock);

        while (window.isOpen()) {
            Input::update();
            window.update();
            // scene.update();
            window.clear(Color(1, 1, 1, 1));
            // window.draw(scene);
            window.display();
        }
    }

    class MyEntity1 : public Drawable, public Transformable
    {
        Sprite _sprite;

    public:
        MyEntity1(Texture& texture)
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
        MyEntity1 mySprite1(assets.greenBlock);

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        mySprite1.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(mySprite1);
            window.display();
        }
    }

    class Scene2 : public Drawable {
        vector<Drawable*> _drawables;
    protected:
        template <class T>
        inline T& add(T* entity) {
            _drawables.push_back(entity);
            return *entity; 
        }
    public:
        virtual ~Scene2() {
            deleteAll(_drawables);
        }

        template <class T, class Arg1> 
        inline T& createDrawable(Arg1& arg1) { return add<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _drawables.size(); i++)
                target.draw(_drawables[i], states);
        }
    };

    void demo2() {
        Window window;
        Assets assets;
        Scene2 scene;
        MyEntity1& myEntity1 = scene.createDrawable<MyEntity1>(assets.greenBlock);

        window.setFramerateLimit(65);
        window.getCamera().setCenter(200);
        myEntity1.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    template <class T> 
    class BaseCtor {
    public:
        virtual T* createConcretely() const = 0;

        T* create() const {
            return createConcretely();
        }
    };

    template <class T, class Arg1>
    class Ctor1 : public BaseCtor<T> {
        Arg1& _arg1;
    public:
        Ctor1(Arg1& arg1)
            : _arg1(arg1)
        { }

        virtual T* createConcretely() const {
            return new T(_arg1);
        }
    };

    template <class T>
    T& test3(const BaseCtor<T>& ctor) {
        return *ctor.create();
    }

    void demo3() {
        Window window;
        Assets assets;

        MyEntity1& myEntity1 = test3<MyEntity1>(Ctor1<MyEntity1, Texture>(assets.greenBlock));

    }

    void run()
    {
        demo3();
        //demo2();
        //demo1();
    }
}
