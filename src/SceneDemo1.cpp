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
    struct is_base_of6 // check if B is a base of D
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

        static D* get(void) { };

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
        const bool test1 = is_base_of6<Entity5, SpriteEntity5>::value;
        test6<test1>();
        const bool test2 = is_base_of6<Entity5, Sprite>::value;
        test6<test2>();
        test6<is_base_of6<Entity5, Sprite>::value>();
        cin.get();
    }

    class Scene7 : public Drawable {
        vector<Entity5*> _entities;
        vector<Drawable*> _drawables;

    protected:
        template <class T, bool>
        struct elementAdder;

        template <class T>
        struct elementAdder<T, true> {
            inline T& add(T* entity, Scene7& scene) {
                scene._entities.push_back(entity);
                return *entity;
            }
        };

        template <class T>
        struct elementAdder<T, false> {
            inline T& add(T* drawable, Scene7& scene) {
                scene._drawables.push_back(drawable);
                return *drawable;
            }
        };

        template<class T>
        inline T& add(T* entity) {
            const bool isEntity = is_base_of6<Entity5, T>::value;
            return elementAdder<T, isEntity>().add(entity, *this);
        }

        template <class T>
        void draw(vector<T*> elements, DrawTarget& target, DrawStates states) {
            for (size_t i = 0; i < elements.size(); i++)
                target.draw(elements[i], states);
        }
    public:
        virtual ~Scene7() {
            deleteAll(_entities);
            deleteAll(_drawables);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return add<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            draw(_entities, target, states);
            draw(_drawables, target, states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo7() {
        Window window;
        Assets assets;
        Scene7 scene;

        window.setFramerateLimit(65);
        SpriteEntity5& spriteEntity = scene.create<SpriteEntity5>(assets.greenBlock);
        Sprite& sprite = scene.create<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        sprite.setScale(100);
        sprite.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    namespace impl {
        template <class T>
        class WrappedDrawable8 : public Entity5 {
            T* _drawable;
        public:
            virtual ~WrappedDrawable8() {
                delete _drawable;
            }

            WrappedDrawable8(T* drawable)
                : _drawable(drawable)
            { }

            virtual void drawSelf(DrawTarget& target, DrawStates states) { }

            virtual void draw(DrawTarget& target, DrawStates states) {
                _drawable->draw(target, states);
            }
        };

        template <class T, bool isEntity>
        struct Wrapper8;

        template <class T>
        struct Wrapper8<T, true> {
            static Entity5* wrap(T* element) { return element; }
        };

        template <class T>
        struct Wrapper8<T, false> {
            static Entity5* wrap(T* element) {
                Entity5* wrappedElem = new WrappedDrawable8<T>(element);
                return wrappedElem;
            }
        };
    }

    class Scene8 : public Drawable {
        vector<Entity5*> _entities;

    protected:
        template<class T>
        inline T& add(T* element) {
            const bool isEntity = is_base_of6<Entity5, T>::value;
            Entity5* wrappedElem = impl::Wrapper8<T, isEntity>::wrap(element);
            _entities.push_back(wrappedElem);

            return *element;
        }

    public:
        virtual ~Scene8() {
            deleteAll(_entities);
        }

        template <class T, class Arg1>
        inline T& create(Arg1& arg1) { return add<T>(new T(arg1)); }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    void demo8() {
        Window window;
        Assets assets;
        Scene8 scene;

        window.setFramerateLimit(65);
        SpriteEntity5& spriteEntity = scene.create<SpriteEntity5>(assets.greenBlock);
        Sprite& sprite = scene.create<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        sprite.setScale(100);
        sprite.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    class BaseEntity9 : public Drawable, public Transformable {
    public:
        virtual void drawSelf(DrawTarget& target, DrawStates states) = 0;

        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            drawSelf(target, states);
        }

        virtual void update() { }
    };

    template <class T>
    class Entity9 : public BaseEntity9 {
        Drawable* _drawable;

    public:
        virtual ~Entity9() {
            delete _drawable;
        }

        virtual void drawSelf(DrawTarget& target, DrawStates states) { }

        virtual void draw(DrawTarget& target, DrawStates states) {
            states.transform *= getTransform();
            _drawable->draw(target, states);
        }

        Entity9(Drawable* drawable) : _drawable(drawable)
        { }

        friend class Scene9;
    };

    class Scene9 : public Drawable {
        vector<BaseEntity9*> _entities;

    protected:
        template <class T>
        inline T& add(T* entity) {
            _entities.push_back(entity);
            return *entity;
        }

    public:
        virtual ~Scene9() {
            deleteAll(_entities);
        }
        
        template <class TEntity, class Arg1>
        inline TEntity& create(Arg1& arg1) {
            return add(new TEntity(arg1));
        }

        template <class TDrawable, class Arg1>
        inline Entity9<TDrawable>& createAsEntity(Arg1& arg1) {
            TDrawable* drawable = new TDrawable(arg1);
            return create<Entity9<TDrawable>>(drawable);
        }

        virtual void draw(DrawTarget& target, DrawStates states = DrawStates::getDefault()) {
            for (size_t i = 0; i < _entities.size(); i++)
                target.draw(_entities[i], states);
        }

        void update() {
            for (size_t i = 0; i < _entities.size(); i++)
                _entities[i]->update();
        }
    };

    class SpriteEntity9 : public BaseEntity9 {
        Sprite _sprite;
    public:
        SpriteEntity9(Texture& tex) : _sprite(tex) { }

        virtual void drawSelf(DrawTarget& target, DrawStates states) {
            target.draw(_sprite, states);
        }

        virtual void update() {
            rotate(0.01f);
        }
    };

    void demo9() {
        Window window;
        Assets assets;
        Scene9 scene;

        window.setFramerateLimit(65);
        SpriteEntity9& spriteEntity = scene.create<SpriteEntity9>(assets.greenBlock);
        Entity9<Sprite>& spriteAsEntity = scene.createAsEntity<Sprite>(assets.yellowBlock);
        spriteEntity.setScale(100);
        spriteAsEntity.setScale(100);
        spriteAsEntity.setPosition(-100);

        while (window.isOpen()) {
            Input::update();
            window.update();
            scene.update();
            window.clear(Color(1, 1, 1, 1));
            window.draw(scene);
            window.display();
        }
    }

    template <template <class T> class TEntity>
    void create10() {
        //Texture tex;
        //auto test = new Entity9<Sprite>(tex);
        // auto test = new TEntity<TDrawable>(tex);
    }

    //template <class SomeType, template <class> class OtherType> class NestedTemplateClass
    //{
    //   
    //};

    void demo10() {
        vector<int> vec;

        //print_container(vec);

        create10<Entity9>();


        // func2<Sprite, Entity9<Sprite>>();

        //test<Entity9<Sprite>>();


        // Assets assets;
        // Scene10 scene;

        // SpriteEntity10& entity = scene.create<SpriteEntity>(assets);
        // Entity10<Sprite>& sprite = scene.create<Entity10<Sprite>>(assets);
    }

    template <class T>
    class Creatable11 {
    public:
        template <class Arg1>
        static T* create(const Arg1& arg1) { return new T(arg1); }
    };

    class MyClass11 : public Creatable11<MyClass11> {
    public:
        MyClass11(int test) { }
    };

    class BaseEntity11 : public Creatable11<BaseEntity11> {

    };

    template <class TDrawable>
    class Entity11 : public BaseEntity11 {

    };

    void demo11() {
        MyClass11::create(42);
    }

    template <class T, class Arg1>
    T& create12(Arg1& arg1) {
        return *(new T(arg1));
    }

    void demo12() {
        Assets assets;
       /* SpriteEntity9& entity = */create12<SpriteEntity9>(assets.yellowBlock);
        Sprite* sprite = new Sprite(assets.greenBlock);
        /*Entity9<Sprite>& spriteAsEntity = */create12<Entity9<Sprite>>(sprite);
    }

    template <class TVal>
    class Arg13 {
        TVal _val;
        TVal& _valRef;
        bool _isRef;
    public:
        Arg13(TVal& val) : _valRef(val), _isRef(true)
        { }

        Arg13(const TVal& val) : _val(val), _valRef(_val), _isRef(false)
        {

        }

        TVal& get() { return _isRef ? _valRef : _val; }
    };

    template <class TArg1>
    class ArgList1_13 {
        Arg13<TArg1> _arg1;

    public:
        ArgList1_13(const Arg13<TArg1>& arg1) : _arg1(arg1)
        { }

        template <class T>
        T* construct() {
            return new T(_arg1.get());
        }
    };

    template <class T>
    Arg13<T> args13(T& t) {
        return Arg13<T>(t);
    }

    template <class T>
    class IntArg {
        T _val;

    public:
        IntArg(T val) : _val(val)
        { }
    };

    template <class T>
    void testOne13(Arg13<T> arg) {

    }

    template <class T>
    void testTwo13(T arg) {

    }

    template <class T>
    void testThree(IntArg<T> arg) {

    }

    class MyClass {
    public: 
        MyClass(int val) {

        }
    };

    template <class TArg1>
    ArgList1_13<TArg1> argList13(TArg1& arg1) {
        return ArgList1_13<TArg1>(arg1);
    }

    template <class TArg1>
    ArgList1_13<TArg1> argList13(const TArg1& arg1) {
        return ArgList1_13<TArg1>(arg1);
    }

    template <class TEntity, class TArgList>
    TEntity& create(TArgList argList) {
        TEntity* entity = argList.template construct<TEntity>();        // https://stackoverflow.com/questions/3786360/confusing-template-error
        return *entity;
    }

    void demo13() {
        testOne13<int>(42);
        testTwo13(42);
        testThree<int>(42);

        int temp = 42;
        Arg13<int> test(temp);

        /*auto test2 = */args13(temp);

        ArgList1_13<int> argList(42);
        argList.construct<MyClass>();

        auto myArgs = argList13(42);
        /*auto myClass = */myArgs.construct<MyClass>();
        /*auto myClass2 =*/ create<MyClass>(argList13(42));

        // scene.create<SpriteEntity>(args(assets.yellowBlock));
    }

    template <class TVal>
    class Arg14 {
        TVal _val;

    public:
        Arg14(const TVal& val) : _val(val)
        { 
        }

        const TVal& get() const { 
            return _val; 
        }
    };

    template <class TArg1>
    class ArgList1_14 {
        const Arg14<TArg1> _arg1;

    public:
        ArgList1_14(const Arg14<TArg1>& arg1) : _arg1(arg1)
        { }

        template <class T>
        T* construct() const {
            return new T(_arg1.get());
        }
    };

    template <class TArg1>
    ArgList1_14<TArg1> args14(const TArg1& arg1) {
        return ArgList1_14<TArg1>(arg1);
    }

    template <class T, class TArgList>
    void create14(const TArgList& argList) {
        argList.template construct<T>();
    }

    class FirstClass14 {
    public:
        FirstClass14(int val)
        { }
    };

    void demo14() {
        create14<FirstClass14>(args14(42));
        // create14<SecondClass14>(args14(test));
        // create14<ThirdClass14>(args14(test2));
    }

    void run()
    {
        demo14();
        //demo13();
        //demo12();
        //demo11();
        //demo10();
        //demo9();
        //demo8();
        //demo7();
        //demo6();
        //demo5();
        //demo4();
        //demo3();
        //demo2();
        //demo1();
    }
}
