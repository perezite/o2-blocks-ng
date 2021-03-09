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
#include <iostream>
#include <string>

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
    class Ctor1_3 : public BaseCtor<T> {
        Arg1& _arg1;
    public:
        Ctor1_3(Arg1& arg1)
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

        /*MyEntity1& myEntity1 = */test3<MyEntity1>(Ctor1_3<MyEntity1, Texture>(assets.greenBlock));
    }

    class MyClass4 {
    public:
        MyClass4(int val)
        {
            cout << val << endl;
        }
    };

    class BaseArgs4 {

    };

    template <class Arg1, class Arg2>
    class Args4 : public BaseArgs4 {
        Arg1* _arg1;
        Arg2* _arg2;

    public:
        inline Arg1& getArg1() { return _arg1; }

        inline Arg2& getArg2() { return _arg2; }

        Args4(Arg1& arg1) : _arg1(&arg1), _arg2(NULL) { };

        Args4(Arg1& arg1, Arg2& arg2) : _arg1(&arg1), _arg2(&arg2) { };
    };

    template <class Arg1> 
    Args4<Arg1, void*> createArgs(Arg1& arg1) { return Args4<Arg1, void*>(arg1); }

    template <class T>
    class Ctor4 {
    public:
        T* create(BaseArgs4& args) {
            return new T();
        }
    };

    template <class T>
    void test4(BaseArgs4& args) {
        T* instance = Ctor4<T>().create(args);
    }

    void demo4() {
        //int test = 42;
        //test4<MyClass4>(createArgs(test));

        //Args4<int, void*> args = createArgs(test);
        //Args args = createArgs()
    }


    template <class T>
    T& test5(const BaseCtor<T>& ctor) {
        return *ctor.create();
    }

    template <class T, class Arg1>
    BaseCtor<T> createCtor(Arg1& arg1) { return Ctor1_3<T, Arg1>(arg1); }

    void demo5() {
        Window window;
        Assets assets;

        test5<MyEntity1>(Ctor1_3<MyEntity1, Texture>(assets.greenBlock));
        //auto temp = Ctor1_3<MyEntity1, Texture>(assets.greenBlock);
    }

    void run()
    {
        //demo5();
        //demo4();
        //demo3();
        //demo2();
        //demo1();
    }
}
