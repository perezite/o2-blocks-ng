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
        Texture yellowBlock;


        Assets()
            :   greenBlock("Textures/GreenBlock.png"),
                yellowBlock("Textures/YellowBlock.png")

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
        inline T& addDrawable(T* entity) {
            _drawables.push_back(entity);
            return *entity; 
        }
    public:
        virtual ~Scene2() {
            deleteAll(_drawables);
        }

        template <class T, class Arg1> 
        inline T& createDrawable(Arg1& arg1) { return addDrawable<T>(new T(arg1)); }

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
    class Entity3 : public T {
    public: 
        Entity3() { }

        template <class Arg1> 
        Entity3(Arg1& arg1) : T(arg1) { }

        template <class Arg1>
        Entity3(const Arg1& arg1) : T(arg1) { }

        virtual void update() {
            cout << "not implemented";
        }
    };

    class MyEntity3 : public Entity3<Sprite> {
    public: 
        MyEntity3() { }

        MyEntity3(Texture& tex)
            : Entity3<Sprite>(tex)
        { }

        virtual void update() {
            rotate(0.01f);
        }
    };

    void demo3() {
        Window window;
        Assets assets;
        Entity3<Sprite> sprite(assets.greenBlock);
        MyEntity3 entity(assets.yellowBlock);

        window.setFramerateLimit(65);
        sprite.setScale(100);
        entity.setPosition(-100);
        entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            sprite.update();
            entity.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(sprite);
            window.draw(entity);
            window.display();
        }
    }

    class Scene4 : public Drawable {
        vector<Drawable*> _drawables;
    protected:
        template <class T>
        inline T& addDrawable(T* entity) {
            _drawables.push_back(entity);
            return *entity;
        }
    public:
        virtual ~Scene4() {
            deleteAll(_drawables);
        }

        template <class T, class Arg1>
        inline T& createDrawable(Arg1& arg1) { return addDrawable<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _drawables.size(); i++)
                target.draw(_drawables[i], states);
        }
    };

    void demo4() {
        Window window;
        Assets assets;
        Scene2 scene;
        // scene.create<MyEntity3>(assets.yellowBlock)
        //MyEntity3 entity(assets.yellowBlock);

        window.setFramerateLimit(65);
        //entity.setPosition(-100);
        //entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            //entity.update();
            window.clear(Color(1, 1, 1, 1));
            //window.draw(entity);
            window.display();
        }
    }

    class Entity5 : public Drawable, public Transformable {
    public:
        virtual void drawSelf(DrawTarget& target, DrawStates states) = 0;
        
        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            drawSelf(target, states);
        }

        virtual void update() { }
    };

    class SpriteEntity5 : public Entity5 {
        Sprite _sprite;
    public:
        SpriteEntity5(Texture& tex) : _sprite(tex) { }
        
        virtual void drawSelf(DrawTarget& target, DrawStates states) {
            target.draw(_sprite, states);
        }

        virtual void update() {
            rotate(0.01f);
        }
    };

    class Scene5 : public Drawable {
        vector<Entity5*> _entities;
    protected:
        template<class T>
        inline T& addEntity(T* entity) {
            _entities.push_back(entity);
            return *entity;
        }
    public:
        virtual ~Scene5() {
            deleteAll(_entities);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return addEntity<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo5() {
        Window window;
        Assets assets;
        Scene5 scene;
        
        window.setFramerateLimit(65);
        SpriteEntity5& entity = scene.create<SpriteEntity5>(assets.greenBlock);
        entity.setScale(100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }


    // https://stackoverflow.com/questions/2631585/c-how-to-require-that-one-template-type-is-derived-from-the-other  
    template <typename B, typename D>
    struct is_base_of // check if B is a base of D
    {
        typedef char yes[1];
        typedef char no[2];

#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wreturn-type"
#endif
        static yes& test(B*) { }
        static no& test(...) { }
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif

        static D* get(void) {};

        static const bool value = (sizeof(test(get())) == sizeof(yes));
    };

    template <bool> void test6();

    template <>
    void test6<true>() {
        cout << "true" << endl;
    }

    template <>
    void test6<false>() {
        cout << "false" << endl;
    }

    void demo6() {
        const bool test1 = is_base_of<Entity5, SpriteEntity5>::value;
        test6<test1>();
        const bool test2 = is_base_of<Entity5, Sprite>::value;
        test6<test2>();
        test6<is_base_of<Entity5, Sprite>::value>();
        cin.get();
    }

    void run()
    {
        demo6();
        //demo5();
        //demo4();
        //demo3();
        //demo2();
        //demo1();
    }
}
