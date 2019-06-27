#include "Window.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "Input.h"
#include "Stopwatch.h"
#include "TextureSheet.h"
#include "Sprite.h"
#include "Math.h"
#include "Tween.h"
#include "ParticleSystem.h"
#include <cstddef>
#include <vector>
#include <map>
#include <algorithm>

void init0(std::vector<sb::Sprite>& stones, sb::Texture& stonesTex) {
    sb::Vector2i size(128, 128);
    sb::Vector2f scale(0.2f, 0.2f);

    stones[0].setTexture(stonesTex, sb::IntRect(0 * size.x, 0 * size.y, size.x, size.y));
    stones[1].setTexture(stonesTex, sb::IntRect(0 * size.x, 1 * size.y, size.x, size.y));
    stones[2].setTexture(stonesTex, sb::IntRect(1 * size.x, 1 * size.y, size.x, size.y));
    stones[3].setTexture(stonesTex, sb::IntRect(2 * size.x, 1 * size.y, size.x, size.y));
    stones[4].setTexture(stonesTex, sb::IntRect(0 * size.x, 2 * size.y, size.x, size.y));
    stones[5].setTexture(stonesTex, sb::IntRect(1 * size.x, 2 * size.y, size.x, size.y));
    stones[6].setTexture(stonesTex, sb::IntRect(2 * size.x, 2 * size.y, size.x, size.y));

    for (std::size_t i = 0; i < stones.size(); i++) {
        sb::Vector2i gridPos(i % 3, i / 3);
        float x = (gridPos.x - 1) * scale.x * 1.1f;
        float y = (gridPos.y - 1) * scale.y * 1.1f;
        stones[i].setPosition(x, y);
        stones[i].setScale(scale);
    }
}

void draw0(sb::DrawBatch& batch, std::vector<sb::Sprite>& stones) {
    for (std::size_t i = 0; i < stones.size(); i++) {
        batch.draw(stones[i]);
    }
}

void demo0() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::DrawBatch batch(512);
    sb::Texture stoneTex;
    std::vector<sb::Sprite> stones(7);

    stoneTex.loadFromAsset("Textures/Stones.png");
    init0(stones, stoneTex);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        draw0(batch, stones);
        window.draw(batch);
        window.display();
    }
}

void init1(sb::Mesh& background, const sb::Camera& camera) {
    sb::Vector2f extent(camera.getWidth() * 0.5f, camera.getWidth() * camera.getInverseAspectRatio() * 0.5f);
    sb::Color bottomColor = sb::Color(252.0f / 255.0f, 182.0f / 255.0f, 159.0f / 255.0f, 1);
    sb::Color topColor = sb::Color(255.0f / 255.0f, 236.0f / 255.0f, 210.0f / 255.0f, 1);
    background[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), bottomColor);
    background[1] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), bottomColor);
    background[2] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), topColor);
    background[3] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), topColor);
}

void demo1() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);

    init1(background, window.getCamera());

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        window.draw(background.getVertices(), sb::PrimitiveType::TriangleStrip);
        window.display();
    }
}

void initCamera(sb::Camera& camera, float minInverseAspectRatio) {
    float minHeight = minInverseAspectRatio;
    float height = camera.getWidth() * camera.getInverseAspectRatio();
    if (height < minHeight) {
        float ratio = minHeight / height;
        camera.setWidth(camera.getWidth() * ratio);
    }
}

void demo2() {
    float inverseAspect = 2 / 4.0f;
    sb::Window window(400, int(400 * inverseAspect));
    sb::Texture coordinatesTex;
    sb::Sprite coordinates;
    sb::Quad reference;

    initCamera(window.getCamera(), 3 / 2.0f);
    coordinatesTex.loadFromAsset("Textures/CoordinateSystem.png");
    coordinates.setTexture(coordinatesTex);
    reference.setScale(0.1f, 1.45f);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(coordinates);
        window.draw(reference);

        window.display();
    }
}

inline float getCameraHeight(sb::Camera& camera) {
    return camera.getWidth() * camera.getInverseAspectRatio();
}

inline sb::Color colorFromRgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
    return sb::Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

class Border : public sb::Drawable, public sb::Transformable {
private:
    sb::Mesh _mesh;
    float _height;

public:
    Border(float height, float thickness)
        : _mesh(10, sb::PrimitiveType::TriangleStrip), _height(height) {
        sb::Vector2f extent(0.5f, height / 2);
        sb::Color innerColor = colorFromRgba(143, 211, 244);
        sb::Color outerColor = colorFromRgba(132, 250, 176);

        _mesh[0] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
        _mesh[1] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
        _mesh[2] = sb::Vertex(sb::Vector2f(+extent.x, -extent.y), outerColor);
        _mesh[3] = sb::Vertex(sb::Vector2f(+extent.x - thickness, -extent.y + thickness), innerColor);
        _mesh[4] = sb::Vertex(sb::Vector2f(+extent.x, +extent.y), outerColor);
        _mesh[5] = sb::Vertex(sb::Vector2f(+extent.x - thickness, +extent.y - thickness), innerColor);
        _mesh[6] = sb::Vertex(sb::Vector2f(-extent.x, +extent.y), outerColor);
        _mesh[7] = sb::Vertex(sb::Vector2f(-extent.x + thickness, +extent.y - thickness), innerColor);
        _mesh[8] = sb::Vertex(sb::Vector2f(-extent.x, -extent.y), outerColor);
        _mesh[9] = sb::Vertex(sb::Vector2f(-extent.x + thickness, -extent.y + thickness), innerColor);
    }

    inline float getHeight() const { return _height; }

    void attachToTop(sb::Camera& targetCamera) {
        float middle = getCameraHeight(targetCamera) / 2 - getHeight() / 2;
        translate(0, middle);
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
        target.draw(_mesh.getVertices(), _mesh.getPrimitiveType(), states);
    }
};

void demo3() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    sb::Mesh background(4, sb::PrimitiveType::TriangleStrip);
    Border border(0.9f * getCameraHeight(window.getCamera()), 0.01f);

    init1(background, window.getCamera());
    border.attachToTop(window.getCamera());

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear();
        window.draw(background.getVertices(), background.getPrimitiveType());
        window.draw(border);
        window.display();
    }
}

class Grid2 : public sb::Drawable {
private:
    sb::Mesh _mesh;

public:
    Grid2(std::size_t numColums, std::size_t numRows, float width, float height, float thickness, const sb::Color& color = sb::Color(1, 0, 0, 1))
        : _mesh((numRows + numColums + 2) * 6, sb::PrimitiveType::TriangleStrip)
    {
        addVerticalLines(numColums + 1, numRows + 1, width, height, thickness, color);
        addHorizontalLines(numRows + 1, width, height, thickness, color);
    }

    void addVerticalLines(std::size_t numColumns, std::size_t indexOffset, float width, float height, float thickness, const sb::Color& color) {
        float delta = width / (numColumns - 1);
        for (std::size_t i = 0; i < numColumns; i++)
            addVerticalLine(i + indexOffset, i * delta - width / 2, height, thickness, color);
    }

    void addVerticalLine(std::size_t index, float horizontalPosition, float height, float thickness, const sb::Color& color) {
        _mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
        _mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, -height / 2), color);
        _mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, -height / 2), color);
        _mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(horizontalPosition + thickness / 2, +height / 2), color);
        _mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
        _mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(horizontalPosition - thickness / 2, +height / 2), color);
    }

    void addHorizontalLines(std::size_t numRows, float width, float height, float thickness, const sb::Color& color) {
        float delta = height / (numRows - 1);
        for (std::size_t i = 0; i < numRows; i++)
            addHorizontalLine(i, i * delta - height / 2, width, thickness, color);
    }

    void addHorizontalLine(std::size_t index, float verticalPosition, float width, float thickness, const sb::Color& color) {
        _mesh[index * 6 + 0] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 1] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 2] = sb::Vertex(sb::Vector2f(-width / 2, verticalPosition + thickness / 2), color);
        _mesh[index * 6 + 3] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition - thickness / 2), color);
        _mesh[index * 6 + 4] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
        _mesh[index * 6 + 5] = sb::Vertex(sb::Vector2f(+width / 2, verticalPosition + thickness / 2), color);
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        target.draw(_mesh.getVertices(), _mesh.getPrimitiveType());
    }
};

void demo4() {
    float aspect = 3 / 2.0f;
    sb::Window window(400, int(400 * aspect));
    Grid2 grid(10, 18, 1, getCameraHeight(window.getCamera()), 0.005f);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(grid);
        window.display();
    }
}

template <class T>
struct Animation {
	sb::Tween<T> tween;
	bool playing = false;
	float t = 0;

	void start() {
		t = 0;
		playing = true;
	}

	void stop() {
		t = tween.getDuration();
		playing = false;
	}

	void update(float ds) {
		if (isPlaying())
			t += ds;
	}

	inline T value() { return tween.value(t); }

	T targetValue() const { return tween.targetValue(); }

	bool isPlaying() { return playing && t < tween.getDuration(); }

};

typedef Animation<float> Animationf;
typedef Animation<sb::Vector2f> Animation2f;

template <class T>
struct Behaviour {
protected:
	T& parent;

	Behaviour(T& parent_) : parent(parent_)
	{ }

	Behaviour(const Behaviour& other) : parent(other.parent) {
	}

	Behaviour& operator=(const Behaviour& other) {
		return *this;
	}
};

class TransformEffects2 : public sb::Transformable, public Behaviour<sb::Transformable> {
	Animation2f _drift;
	Animationf _spin;

	Animation2f _driftOld;
	Animationf _spin2;
	Animationf _wobble;

public:
	TransformEffects2(sb::Transformable& parent) : Behaviour(parent) { }

	inline bool isMoving() { return _driftOld.isPlaying(); }

	inline bool isRotating() { return _spin2.isPlaying(); }

	inline bool isScaling() { return _wobble.isPlaying(); }

	sb::Vector2f getTargetPosition() { 
		return _driftOld.isPlaying() ? _driftOld.targetValue() : parent.getPosition(); 
	}

	float getTargetRotation() { 
		return _spin2.isPlaying() ? _spin2.targetValue() : parent.getRotation(); 
	}

	sb::Vector2f getTargetScale() { 
		return _wobble.isPlaying() ? sb::Vector2f(_wobble.targetValue()) : parent.getScale(); 
	}

	void driftOld(const sb::Vector2f& target, float duration = 0.15f) {
		const sb::Vector2f& pos = parent.getPosition();
		_driftOld.tween = sb::Tween2f().sineOut(pos, target, duration);
		_driftOld.start();
	}
	
	void driftTo(const sb::Vector2f& target, float duration = 0.15f) {
		sb::Vector2f effectPosition = parent.getPosition() + _drift.value();
		parent.setPosition(target);
		sb::Vector2f offset = effectPosition - parent.getPosition();
		_drift.tween = sb::Tween2f().sineOut(offset, sb::Vector2f(0), duration);
		_drift.start();
	}

	inline void driftBy(const sb::Vector2f& amount, float duration = 0.15f) {
		driftTo(parent.getPosition() + amount, duration);
	}

	void bounce(const sb::Vector2f& target, float duration = 0.5f) {
		const sb::Vector2f& pos = parent.getPosition();
		_driftOld.tween = sb::Tween2f().bounceOut(pos, target, duration);
		_driftOld.start();
	}

	void spinOld(float radians) {
		_spin2.tween = sb::Tweenf().bounceOut(parent.getRotation(), radians, 0.5f);
		_spin2.start();
	}

	void spinTo(float radians, float duration = 0.5f) {
		float effectRotation = parent.getRotation() + _spin.value();
		float offset = effectRotation - radians;
		parent.setRotation(radians);
		_spin.tween = sb::Tweenf().bounceOut(offset, 0, duration);
		_spin.start();
	}

	void spinBy(float radians, float duration = 0.5f) {
		spinTo(parent.getRotation() + radians, duration);
		//float effectRotation = parent.getRotation() + _spin.value();
		//float offset = effectRotation - radians;
		//parent.setRotation(radians);
		//_spin.tween = sb::Tweenf().bounceOut(offset, 0, duration);
		//_spin.start();
	}

	void pop() {
		if (!_wobble.isPlaying()) {
			float scale = parent.getScale().x;
			_wobble.tween = sb::Tweenf().quintInOut(scale, 1.4f * scale, 0.2f)
				.expoOut(1.4f * scale, scale, 0.4f);
			_wobble.start();
		}
	}

	void die(float duration = 0.8f) {
		_wobble.tween = sb::Tweenf().backInOut(parent.getScale().x, 0, duration);
		float angle = parent.getRotation();
		_spin2.tween = sb::Tweenf().backInOut(angle, angle + sb::random(-90, 90) * sb::ToRadian, 0.3f * duration);
		_wobble.start();
		_spin2.start();
	}

	void update(float ds) {
		SB_ERROR("deprecated");

		_drift.update(ds);
		_spin.update(ds);

		if (_drift.isPlaying())
			setPosition(_drift.value());
		if (_spin.isPlaying())
			setRotation(_spin.value());

		_driftOld.update(ds);
		_wobble.update(ds);
		_spin2.update(ds);

		if (_wobble.isPlaying())
			parent.setScale(_wobble.value());
		if (_driftOld.isPlaying())
			parent.setPosition(_driftOld.value());
		if (_spin2.isPlaying())
			parent.setRotation(_spin2.value());
	}

};

class TransformEffects : public sb::Transformable {
	Animation2f _drift;
	Animationf _wobble;
	Animationf _spin;

protected:
	void smoothMoveTo(const sb::Vector2f& end, sb::Transformable& target, sb::Tween2f& normalizedTween, float duration) {
		sb::Vector2f effectPosition = target.getPosition() + _drift.value();
		target.setPosition(end);
		sb::Vector2f offset = effectPosition - end;
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_drift.tween = normalizedTween;
		_drift.start();
	}

	inline void smoothMoveBy(const sb::Vector2f& amount, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		driftTo(target.getPosition() + amount, target, duration);
	}

	void smoothRotateTo(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		float effectRotation = target.getRotation() + _spin.value();
		float offset = effectRotation - radians;
		target.setRotation(radians);
		normalizedTween.stretchDuration(duration);
		normalizedTween.scale(offset);
		_spin.tween = normalizedTween;
		_spin.start();
	}

	inline void smoothRotateBy(float radians, sb::Transformable& target, sb::Tweenf& normalizedTween, float duration) {
		smoothRotateTo(target.getRotation() + radians, target, normalizedTween, duration);
	}

public:
	bool isPlaying() {
		return _drift.isPlaying() || _wobble.isPlaying() || _spin.isPlaying();
	}

	inline void driftTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.2f) {
		sb::Tween2f tween = sb::Tween2f().sineOut(sb::Vector2f(1), sb::Vector2f(0), 1);
		smoothMoveTo(end, target, tween, duration);
	}

	inline void driftBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
		driftTo(target.getPosition() + amount, target, duration);
	}

	void bounceTo(const sb::Vector2f& end, sb::Transformable& target, float duration = 0.5f) {
		sb::Tween2f tween = sb::Tween2f().bounceOut(sb::Vector2f(1), sb::Vector2f(0), 1);
		smoothMoveTo(end, target, tween, duration);
	}

	inline void bounceBy(const sb::Vector2f& amount, sb::Transformable& target, float duration = 0.2f) {
		bounceTo(target.getPosition() + amount, target, duration);
	}

	void pop(sb::Transformable& target, float duration = 0.15f) {
		_wobble.tween = sb::Tweenf().quintInOut(_wobble.value(), 0.4f, 0.2f).expoOut(0.4f, 0, 0.4f);
		_wobble.start();
	}

	void spinTo(float radians, sb::Transformable& target, float duration = 0.5f) {
		sb::Tweenf tween = sb::Tweenf().bounceOut(1, 0, 1);
		smoothRotateTo(radians, target, tween, duration);
	}

	void spinBy(float radians, sb::Transformable& target, float duration = 0.5f) {
		spinTo(target.getRotation() + radians, target, duration);
	}

	void implode(sb::Transformable& target, float duration = 0.8f) {
		_wobble.tween = sb::Tweenf().backInOut(_wobble.value(), -1, duration);
		_wobble.start();

		sb::Tweenf tween = sb::Tweenf().backInOut(1, 0, 1);
		smoothRotateBy(sb::random(-90, 90) * sb::ToRadian, target, tween, duration * 0.3f);
	}

	void apply(sb::Transform& transform) {
		sb::Transform effectRotation(sb::Vector2f(0), sb::Vector2f(1), _spin.value());
		sb::Transform effectTranslation(_drift.value(), sb::Vector2f(1), 0);
		sb::Transform effectScale(sb::Vector2f(0), (1 + _wobble.value()) * sb::Vector2f(1), 0);
		transform = effectTranslation * transform * effectRotation * effectScale;
	}

	void stop() {
		_drift.stop();
		_wobble.stop();
		_spin.stop();
	}

	void update(float ds) {
		_drift.update(ds);
		_wobble.update(ds);
		_spin.update(ds);
	}
};

class Light {
    sb::Vector2f _direction;

public: 
    Light(const sb::Vector2f& direction = sb::Vector2f(0, -1)) : _direction(direction)
    { }

    inline void setDirection(const sb::Vector2f& direction) { _direction = direction; }

    inline const sb::Vector2f& getDirection() const { return _direction; }
};

class Block : public sb::Drawable, public sb::Transformable {
public:
	enum struct State {
		Alive, Dying, Garbage
	};

private:
    sb::Sprite _sprite;
    const Light* _light;
	State _state;
	TransformEffects _effects;
	TransformEffects2 _effects2;

protected:
    static sb::Texture& getSheet() {
        static sb::Texture texture("Textures/Blocks.png");
        return texture;
    }

    inline const sb::IntRect getTextureArea(std::size_t x, std::size_t y) {
        static sb::Vector2i blockSize(128, 128);
        return sb::IntRect(x * blockSize.x, y * blockSize.y, blockSize.x, blockSize.y);
    }

    void setTexture(char type) {
        if (type == 'i')
            _sprite.setTexture(getSheet(), getTextureArea(1, 2));
        else if (type == 'j')
            _sprite.setTexture(getSheet(), getTextureArea(2, 2));
		else if (type == 'l')
			_sprite.setTexture(getSheet(), getTextureArea(1, 1));
		else if (type == 'o')
			_sprite.setTexture(getSheet(), getTextureArea(0, 2));
		else if (type == 's')
			_sprite.setTexture(getSheet(), getTextureArea(0, 1));
		else if (type == 't')
			_sprite.setTexture(getSheet(), getTextureArea(0, 0));
		else if (type == 'z')
			_sprite.setTexture(getSheet(), getTextureArea(2, 1));
        else if (type == 'm')
            _sprite.setTexture(getSheet(), getTextureArea(1, 2));
        else
            SB_ERROR("Invalid Tetromino type " << type);
    }

    sb::Vector2f transformDirection(const sb::Transform& transform, sb::Vector2f& dir) {
        const float* m = transform.getMatrix();
        return sb::Vector2f(
            m[0] * dir.x + m[3] * dir.y,
            m[1] * dir.x + m[4] * dir.y);
    }

    void updateLighting(const sb::Transform& transform) {
        if (_light) {
            sb::Vector2f up(0, 1);
            sb::Vector2f transformedUp = transformDirection(transform, up);
            float angle = sb::angle(-_light->getDirection(), transformedUp) + 45 * sb::ToRadian;
            angle = angle < 0 ? angle + 2 * sb::Pi : angle;
            int steps = int(angle / (90 * sb::ToRadian));
             _sprite.setRotation(-steps * 90 * sb::ToRadian);
        }
    }

	void die(float ds) {
		if (!_effects.isPlaying())
			_state = State::Garbage;
	}

public:
    Block(char type = 'i') : _light(NULL), _state(State::Alive), _effects2(*this)
    {
        setType(type);
    }

	inline const State& getState() const { return _state; }

	inline TransformEffects2& getEffects2() { SB_WARNING("Obsolete!") ; return _effects2; }

	inline TransformEffects& getEffects() { return _effects; }

	inline void setColor(const sb::Color& color) { _sprite.setColor(color); }

    void setType(char type) {
        type = tolower(type);
        setTexture(type);
    }

    inline void setLight(const Light& light) { _light = &light; }

	void implode(float duration = 0.8f) {
		getEffects().implode(*this, duration);
		_state = State::Dying;
	}

	void update(float ds) {
		_effects.update(ds);
		if (_state == State::Dying)
			die(ds);
	}

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
        updateLighting(states.transform);
        target.draw(_sprite, states);
    }
};

void init5(std::vector<Block>& blocks) {
    static std::vector<char> types = { 'i', 'j', 'm' };
    for (size_t i = 0; i < blocks.size(); i++) {
        blocks[i].setType(types[rand() % types.size()]);
        blocks[i].setPosition(sb::random2D(-0.5f, 0.5f));
        blocks[i].setScale(sb::random(0.1f, 0.2f));
    }
}

void draw5(sb::DrawBatch& batch, std::vector<Block>& blocks) {
    for (size_t i = 0; i < blocks.size(); i++)
        batch.draw(blocks[i]);
}

void demo5() {
    float aspect = 3 / 2.0f;
    sb::DrawBatch batch(512);
    sb::Window window(400, int(400 * aspect));
    std::vector<Block> blocks(20);

    init5(blocks);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        draw5(batch, blocks);
        window.draw(batch);
        window.display();
    }
}

inline int getWindowHeight(int width) {
    float aspect = 3 / 2.0f;
    return int(width * aspect);
}

template <class V>
void computeExtents(std::vector<V> v, V& min, V& max) {
	min = max = V(v[0].x, v[0].y);

	for (size_t i = 0; i < v.size(); i++) {
		V& val = v[i];
		if (val.x < min.x)
			min.x = val.x;
		if (val.x > max.x)
			max.x = val.x;
		if (val.y < min.y)
			min.y = val.y;
		if (val.y > max.y)
			max.y = val.y;
	}
}

sb::FloatRect getTransformedBounds(sb::FloatRect& bounds, const sb::Transform& transform) {
	std::vector<sb::Vector2f> edges(4);
	edges[0] = sb::Vector2f(bounds.left, bounds.bottom);
	edges[1] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom);
	edges[2] = sb::Vector2f(bounds.left, bounds.bottom + bounds.height);
	edges[3] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom + bounds.height);
	for (size_t i = 0; i < edges.size(); i++)
		edges[i] *= transform;

	sb::Vector2f min, max;
	computeExtents(edges, min, max);
	return sb::FloatRect(min.x, min.y, max.x - min.x, max.y - min.y);
}

class Tetromino : public sb::Drawable, public sb::Transformable {
	char _type;
    std::vector<Block> _blocks;
    std::vector<sb::Vector2i> _blockPositions;
    sb::IntRect _blockBounds;
	TransformEffects _effects;
	TransformEffects2 _effects2;

protected:
    void clear() {
        _blocks.clear();
        _blockPositions.clear();	
    }

    void computeBlockBounds() {
        sb::Vector2i minimum, maximum;
        computeExtents(_blockPositions, minimum, maximum);
        _blockBounds = sb::IntRect(minimum.x, minimum.y, maximum.x - minimum.x + 1, maximum.y - minimum.y + 1);
    }

    void createBlocks(const std::vector<sb::Vector2i>& positions, char type) {
        computeBlockBounds();
        for (size_t i = 0; i < positions.size(); i++) {
            Block block;
            block.setPosition((float)positions[i].x, (float)positions[i].y);
            block.setType(type);
            _blocks.push_back(block);
        }
    }

    void createBlocks(char type) {
		if (type == 'i')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0) };
		else if (type == 'j')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(1, -1) };
		else if (type == 'l')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(-1, -1) };
		else if (type == 'o')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, 1), sb::Vector2i(1, 1) };
		else if (type == 's')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1), sb::Vector2i(-1, -1) };
		else if (type == 't')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };
        else if (type == 'z')
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(0, -1), sb::Vector2i(1, -1) };
        else if (type == 'm')
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0) };

        createBlocks(_blockPositions, type);
    }

public:
    Tetromino(char type = 'i') : _effects2(*this) {
        setType(type);
    }

	inline TransformEffects& getEffects() { return _effects; }

	inline TransformEffects2& getEffects2() { SB_WARNING("obsolete"); return _effects2; }

	inline char getType() const { return _type; }

	inline static const std::vector<char> getTypes() {
		return{ 'i', 'j', 'l', 'o', 's', 't', 'z' };
	}

	const std::vector<sb::Vector2f> getBlockPositions() {
		std::vector<sb::Vector2f> blockPositions;
		blockPositions.reserve(_blocks.size());
		for (size_t i = 0; i < _blocks.size(); i++)
			blockPositions.push_back(getTransform() * _blocks[i].getPosition());

		return blockPositions;
	}

	void setColor(const sb::Color& color) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].setColor(color);
	}

    void setType(char type) {
        clear();
		_type = tolower(type);
        createBlocks(_type);
    }

    inline void setLight(Light& light) { 
        for (std::size_t i = 0; i < _blocks.size(); i++)
            _blocks[i].setLight(light);
    }

	inline const sb::IntRect& getBlockBounds() const { return _blockBounds; }

    sb::FloatRect getBounds() {
        sb::Vector2f blockSize(1.f, 1.f);
        sb::FloatRect bounds(_blockBounds.left - 0.5f, _blockBounds.bottom - 0.5f, (float)_blockBounds.width, (float)_blockBounds.height);

		return getTransformedBounds(bounds, getTransform());
    }

	void update(float ds) {
		_effects.update(ds);
		//_effects2.update(ds);
	}

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
        for (size_t i = 0; i < _blocks.size(); i++)
            target.draw(_blocks[i], states);
    }
};

void init6(std::vector<Tetromino>& tetrominos) {
    srand(385949385);
    static std::vector<char> types = { 'i', 'j' };
    for (size_t i = 0; i < tetrominos.size(); i++) {
        tetrominos[i].setType(types[rand() % types.size()]);
        tetrominos[i].setPosition(sb::random2D(-0.5f, 0.5f));
        tetrominos[i].setScale(sb::random(0.01f, 0.1f) * tetrominos[i].getScale());
    }
}

void draw6(sb::DrawBatch& batch, std::vector<Tetromino>& tetrominos) {
    for (size_t i = 0; i < tetrominos.size(); i++)
        batch.draw(tetrominos[i]);
}

void demo6() {
    sb::DrawBatch batch(512);
    sb::Window window(400, getWindowHeight(400));
    std::vector<Tetromino> tetrominos(10);

    init6(tetrominos);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        draw6(batch, tetrominos);
        window.draw(batch);
        window.display();
    }
}

namespace {
    float getSeconds() {
        static sb::Stopwatch sw;
        return sw.getElapsedSeconds();
    }

    float getDeltaSeconds() {
        static float lastElapsed = 0;
        float elapsed = getSeconds();
        float delta = elapsed - lastElapsed;
        lastElapsed = elapsed;
        return delta;
    }
}

void update7(sb::Triangle& spotlight, Light& light, Block& block) {
    float t = getSeconds();
    sb::Vector2f relativePosition(0.2f * cos(-t), 0.2f * sin(-t));
    spotlight.setPosition(block.getPosition() + relativePosition);
    light.setDirection(-relativePosition);
}

void demo7() {
    sb::Window window(400, getWindowHeight(400));
    Light light1;
    Light light2;
    Block block1('i');
    Block block2('j');
    sb::Triangle spotlight1;

    block1.setScale(0.2f, 0.2f);
    block1.setPosition(-0.2f, 0.2f);
    block1.setLight(light1);
    block2.setScale(0.2f, 0.2f);
    block2.setPosition(0.2f, -0.2f);
    block2.setLight(light2);
    spotlight1.setScale(0.05f, 0.05f);

    while (window.isOpen()) {
        float ds = getDeltaSeconds();
        sb::Input::update();
        window.update();
        update7(spotlight1, light1, block1);
        block2.rotate(ds);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(block1);
        window.draw(block2);
        window.draw(spotlight1);
        window.display();
    }
}

void init8a(Tetromino& tetromino, sb::Triangle& spotlight, Light& light) {
    tetromino.setScale(0.1f * tetromino.getScale());
    tetromino.setPosition(-0.2f, 0.2f);
    tetromino.setLight(light);
    spotlight.setScale(0.05f, 0.05f);
}

void init8b(Tetromino& tetromino, Light& light) {
    tetromino.setScale(0.1f * tetromino.getScale());
    tetromino.setPosition(0.2f, -0.2f);
    tetromino.setLight(light);
}

void update8(sb::Triangle& spotlight, Light& light, Tetromino& tetromino) {
    float t = getSeconds();
    sb::Vector2f relativePosition(0.2f * cos(-t), 0.2f * sin(-t));
    spotlight.setPosition(tetromino.getPosition() + relativePosition);
    light.setDirection(-relativePosition);
}

void demo8() {
    sb::Window window(400, getWindowHeight(400));
    Light light1;
    Light light2;
    sb::Triangle spotlight;
    Tetromino tetromino1('i');
    Tetromino tetromino2('j');

    init8a(tetromino1, spotlight, light1);
    init8b(tetromino2, light2);

    while (window.isOpen()) {
        float ds = getDeltaSeconds();
        sb::Input::update();
        window.update();
        update8(spotlight, light1, tetromino1);
        tetromino2.rotate(ds);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(tetromino1);
        window.draw(tetromino2);
        window.draw(spotlight);
        window.display();
    }
}

class Line : public sb::Drawable, public sb::Transformable {
    std::vector<sb::Vector2f> _points;
    float _thickness;
    sb::Color _color;
    std::vector<sb::Vertex> _vertices;
    bool _verticesNeedUpdate;

protected:
    void addSegment(const sb::Vector2f& start, const sb::Vector2f& end) {
        float half = _thickness / 2;
        sb::Vector2f dist = end - start;
        sb::Vector2f perp = sb::Vector2f(-dist.y, dist.x).normalized();
        std::vector<sb::Vertex> segment(6);

        segment[0] = sb::Vertex(start - half * perp, _color);
        segment[1] = segment[0];
        segment[2] = sb::Vertex(start + half * perp, _color);
        segment[3] = sb::Vertex(end - half * perp, _color);
        segment[4] = sb::Vertex(end + half * perp, _color);
        segment[5] = segment[4];

        _vertices.insert(_vertices.end(), segment.begin(), segment.end());
    }

	void updateVertices() {
		_vertices.clear();
		for (size_t i = 1; i < _points.size(); i++)
			addSegment(_points[i - 1], _points[i]);

		_verticesNeedUpdate = false;
	}

public:
    Line(float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1)) 
        : _thickness(thickness), _color(color), _verticesNeedUpdate(false) 
    { }

	const std::vector<sb::Vertex>& getVertices() { 
		if (_verticesNeedUpdate)
			updateVertices();
		return _vertices; 
	}

	inline void addPoint(const sb::Vector2f& point) { addPoint(point.x, point.y); }

    void addPoint(float x, float y) {
        _points.push_back(sb::Vector2f(x, y));
        _verticesNeedUpdate = true;
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
        if (_verticesNeedUpdate)
            updateVertices();

        target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
    }
};

void computeOutline(const sb::FloatRect rect, Line& line) {
    line.addPoint(rect.left, rect.bottom);
    line.addPoint(rect.right(), rect.bottom);
    line.addPoint(rect.right(), rect.top());
    line.addPoint(rect.left, rect.top());
    line.addPoint(rect.left, rect.bottom);
}

void demo9() {
    sb::Window window(400, getWindowHeight(400));
    Line line(0.01f);
    sb::FloatRect bounds(-0.3f, -0.4f, 0.2f, 0.3f);
    computeOutline(bounds, line);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(line);
        window.display();
    }
}

void drawOutline(sb::DrawTarget& target, const sb::FloatRect& rect, float thickness = 0.01f, const sb::Color& color = sb::Color(1, 0, 0, 1)) {
	Line outline(thickness, color);
	computeOutline(rect, outline);
	target.draw(outline);
}

bool isTouchGoingDown(sb::Window& window, Tetromino& tetromino) {
	if (sb::Input::isTouchGoingDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (tetromino.getBounds().contains(touch))
			return true;
	}

	return false;
}

void demo10() {
    sb::Window window(400, getWindowHeight(400));
    Light light;
    Tetromino tetromino('j');

	tetromino.setPosition(0.2f, 0.2f);
    tetromino.setScale(0.15f);
    tetromino.setLight(light);

    while (window.isOpen()) {
        sb::Input::update();
        window.update();
        if (isTouchGoingDown(window, tetromino))
            tetromino.rotate(-90 * sb::ToRadian);

        window.clear(sb::Color(1, 1, 1, 1));
        window.draw(tetromino);
		drawOutline(window, tetromino.getBounds(), 0.01f);

        window.display();
    }
}

bool isTouchDown(sb::Window& window, Tetromino& tetromino) {
	if (sb::Input::isTouchDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (tetromino.getBounds().contains(touch))
			return true;
	}

	return false;
}

void demo11() {
	sb::Window window(400, getWindowHeight(400));
	Light light;
	Tetromino tetromino('j');

	tetromino.setScale(0.15f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (isTouchDown(window, tetromino))
			tetromino.setPosition(sb::Input::getTouchPosition(window).x, tetromino.getPosition().y);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		drawOutline(window, tetromino.getBounds(), 0.01f);
		
		window.display();
	}
}

class Grid : public sb::Drawable, public sb::Transformable {
	sb::Vector2i _gridSize;
	float _thickness;
	sb::Color _color;
	std::vector<sb::Vertex> _vertices;

protected:
	void computeLine(const sb::Vector2f& start, const sb::Vector2f& end) {
		Line line(_thickness, _color);
		line.addPoint(start);
		line.addPoint(end);
		_vertices.insert(_vertices.end(), line.getVertices().begin(), line.getVertices().end());
	}

	inline sb::Vector2f getSize() {
		return sb::Vector2f(1, _gridSize.y / float(_gridSize.x));
	}

	void computeVerticalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.x / _gridSize.x;
		for (int i = 0; i <= _gridSize.x; i++) {
			sb::Vector2f start(i * delta - halfSize.x, -halfSize.y);
			sb::Vector2f end(i * delta - halfSize.x, +halfSize.y);
			computeLine(start, end);
		}
	}

	void computeHorizontalLines() {
		sb::Vector2f size = getSize();
		sb::Vector2f halfSize = 0.5f * size;
		float delta = size.y / _gridSize.y;
		for (int i = 0; i <= _gridSize.y; i++) {
			sb::Vector2f start(-halfSize.x, i * delta - halfSize.y);
			sb::Vector2f end(+halfSize.x, i * delta -halfSize.y);
			computeLine(start, end);
		}
	}

	void computeLines() {
		_vertices.clear();
		sb::Vector2f delta(1.f / _gridSize.x, 1.f / _gridSize.y);
		
		computeVerticalLines();
		computeHorizontalLines();
	}

public:
	Grid(sb::Vector2i gridSize, float thickness = 0.1f, const sb::Color& color = sb::Color(1, 0, 0, 1)) 
		: _gridSize(gridSize), _thickness(thickness), _color(color)
	{
		computeLines();
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_vertices, sb::PrimitiveType::TriangleStrip, states);
	}
};

void demo12() {
	sb::Window window(400, getWindowHeight(400));
	Grid grid(sb::Vector2i(10, 15), 0.005f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(grid);
		window.display();
	}
}

void demo13() {
	sb::Window window(400, getWindowHeight(400));
	Tetromino tetromino('m');

	tetromino.setScale(0.25f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

class Board : public sb::Drawable, public sb::Transformable {
	sb::DrawBatch _batch;
	sb::Vector2i _boardSize;
	Grid _grid;
	bool _isGridEnabled;
	std::vector<Block> _blocks;
	Light _light;
	Tetromino _tetromino;
	bool _hasTetromino;
	float _stepIntervalInSeconds;
	float _secondsSinceLastStep;
	bool _isDead;
	size_t _linesCleared;

protected:

	void createTetromino(Tetromino& tetromino, const sb::Vector2i& boardPosition) {
		tetromino.setPosition(boardToWorldPosition(boardPosition));
		tetromino.setScale(getCellSize());
		_tetromino = tetromino;
		_tetromino.setLight(_light);
		_hasTetromino = true;
	}

	void death() {
		std::cout << "You, sir, are a dead man!" << std::endl;
		_hasTetromino = false;
		_isDead = true;
	}

	void createRandomTetromino() {
		const std::vector<char> types = Tetromino::getTypes();
		createTetromino(types[rand() % types.size()]);
		if (isInvalid(_tetromino))
			death();
	}

	sb::Vector2f boardToWorldPosition(const sb::Vector2i& boardPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f halfSize = 0.5f * size;
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		return sb::Vector2f(-halfSize.x + (boardPos.x + 0.5f) * delta.x,
			-halfSize.y + (boardPos.y + 0.5f) * delta.y);
	}

	inline int floori(float x) { return int(floor(x)); }

	sb::Vector2i worldToBoardPosition(const sb::Vector2f& worldPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		sb::Vector2f adjustedPos = worldPos + 0.5f * size;
		auto result = sb::Vector2i(floori(adjustedPos.x / delta.x), floori(adjustedPos.y / delta.y));
		return result;
	}

	sb::Vector2f getCellSize() {
		return sb::Vector2f(1.f / _boardSize.x, 1.f / _boardSize.x);
	}

	void moveBy(Tetromino& tetromino, const sb::Vector2i& translation) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.setPosition(boardToWorldPosition(position));
	}

	void bounceBy(Tetromino& tetromino, const sb::Vector2i& translation) {
		sb::Vector2i position = worldToBoardPosition(tetromino.getPosition());
		position += translation;
		tetromino.getEffects().bounceTo(boardToWorldPosition(position), tetromino);
		// tetromino.setPosition(boardToWorldPosition(position));
	}

	bool isOccupied(const sb::Vector2i& position) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			const sb::Vector2i blockPosition = worldToBoardPosition(_blocks[i].getPosition());
			bool isBlockAlive = _blocks[i].getState() == Block::State::Alive;
			if (blockPosition == position && isBlockAlive)
				return true;
		}

		return false;
	}

	bool isOutsideBoard(const sb::Vector2i& position) {
		return position.x < 0 || position.x >= _boardSize.x || 
			position.y < 0 || position.y >= _boardSize.y;
	}

	bool isInvalid(Tetromino& tetromino) {
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();

		for (std::size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			if (isOccupied(boardPosition) || isOutsideBoard(boardPosition))
				return true;
		}

		return false;
	}

	inline bool isValid(Tetromino& tetromino) { 
		return !isInvalid(tetromino); 
	}

	void freeze(Tetromino& tetromino) {
		char type = tetromino.getType();
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();
		_hasTetromino = false;

		for (size_t i = 0; i < blockPositions.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(blockPositions[i]);
			createBlock(type, boardPosition);
		}
	}

	bool isLineFull(size_t y) {
		for (int x = 0; x < _boardSize.x; x++) {
			if (!isOccupied(sb::Vector2i(x, y)))
				return false;
		}

		return true;
	}

	void implodeLine(size_t y) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y == y) {
				float duration = isOccupied(sb::Vector2i(boardPos.x, boardPos.y + 1)) ? 0.01f : 0.8f;
				_blocks[i].implode(duration);
			}
		}
	}
	
	void dropBlocksAboveLine(int y) {
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPos = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPos.y > y) 
				bounceBy(_blocks[i], sb::Vector2i(0, -1));
		}	
	}

	void clearLine(size_t y) {
		implodeLine(y);
		dropBlocksAboveLine(y);
		_linesCleared++;
	}

	void clearLines() {
		for (int y = 0; y < _boardSize.y; y++) {
			while (isLineFull(y))
				clearLine(y);
		}
	}

	void drop(Tetromino& tetromino) {
		bounceBy(tetromino, sb::Vector2i(0, -1));

		if (isInvalid(tetromino)) {
			//std::cout << "A collision, sir" << std::endl;
			//tetromino.setPosition(previousPosition);
			bounceBy(tetromino, sb::Vector2i(0, 1));
			freeze(tetromino);
			clearLines();
			createRandomTetromino();
		}
	}

	void step(Tetromino& tetromino, float ds) {
		_secondsSinceLastStep += ds;

		while (_secondsSinceLastStep >= _stepIntervalInSeconds) {
			drop(tetromino);
			_secondsSinceLastStep -= _stepIntervalInSeconds;
		}
	}

	bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos, const sb::Vector2i& dir) {
		sb::Vector2i boardPosTo = worldToBoardPosition(targetPos);
		Tetromino testTetromino = tetromino;

		while (isValid(testTetromino)) {
			sb::Vector2i pos = worldToBoardPosition(testTetromino.getPosition());
			if (pos == boardPosTo)
				return true;
			moveBy(testTetromino, dir);
		}

		return false;
	}

	bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos) {
		static const std::vector<sb::Vector2i> directions = { sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };

		for (size_t i = 0; i < directions.size(); i++) {
			if (isReachable(tetromino, targetPos, directions[i]))
				return true;
		}

		return false;
	}

	static bool isGarbage(const Block& block) { 
		return block.getState() == Block::State::Garbage; 
	}

	void dispose() {
		_blocks.erase(std::remove_if(_blocks.begin(), _blocks.end(), isGarbage), _blocks.end());	
	}

	void bounceBy(Block& block, const sb::Vector2i& translation, float duration = 0.5f) {
		sb::Vector2i position = worldToBoardPosition(block.getPosition());
		position += translation;
		block.getEffects().bounceTo(boardToWorldPosition(position), block, duration);
	}

	int getLowestLineWithBlocks() {
		int lowest = -1;
		for (size_t i = 0; i < _blocks.size(); i++) {
			sb::Vector2i boardPosition = worldToBoardPosition(_blocks[i].getPosition());
			if (boardPosition.y < lowest || lowest < 0)
				lowest = boardPosition.y;
		}

		return lowest;
	}

public:
	Board(const sb::Vector2i& boardSize) 
		: _batch(1024), _boardSize(boardSize), _grid(boardSize, 0.01f), 
		_isGridEnabled(false), _hasTetromino(false), _stepIntervalInSeconds(0.5f), 
		_secondsSinceLastStep(0), _isDead(false), _linesCleared(0)
	{ }

	inline bool hasTetromino() const { return _hasTetromino; }

	inline Tetromino& getTetromino() { return _tetromino; }

	inline std::vector<Block>& getBlocks() { return _blocks; }

	inline const sb::Vector2i& getBoardSize() const { return _boardSize; }

	inline size_t getLinesCleared() const { return _linesCleared; }

	sb::Vector2f getSize() const {
		float inverseAspect = _boardSize.y / float(_boardSize.x);
		return sb::Vector2f(getScale().x * 1.f, getScale().y * inverseAspect);
	}

	sb::FloatRect getBounds() {
		sb::Vector2f size = getSize();
		sb::FloatRect bounds(-size.x / 2, -size.y / 2, size.x, size.y);
		
		return getTransformedBounds(bounds, getTransform());
	}

	inline void enableGrid(bool enabled) { _isGridEnabled = enabled; }

	inline void setStepInterval(float dropIntervalInSeconds) { _stepIntervalInSeconds = dropIntervalInSeconds; }

	void createBlock(char type, const sb::Vector2i& position) {
		Block block(type);
		block.setPosition(boardToWorldPosition(position));
		block.setScale(getCellSize());
		_blocks.push_back(block);
	}

	void createTetromino(char type, const sb::Vector2i& boardPosition) {
		Tetromino tetromino(type);
		createTetromino(tetromino, boardPosition);
	}

	void createTetromino(char type) {
		Tetromino tetromino(type);
		sb::Vector2i boardPosition(_boardSize.x / 2 - (_boardSize.x % 2 ? 0 : 1), 
			_boardSize.y - tetromino.getBlockBounds().top());
		createTetromino(tetromino, boardPosition);
	}

	void driftTetrominoTo(sb::Vector2f pos) {
		sb::Vector2f previousPos = _tetromino.getPosition();
		sb::Vector2f newPos = getTransform() * pos;
		newPos.y = std::min(previousPos.y, newPos.y);

		if (isReachable(_tetromino, newPos)) {
			sb::Vector2i boardPos = worldToBoardPosition(newPos);
			sb::Vector2f worldPos = boardToWorldPosition(boardPos);
			_tetromino.getEffects().driftTo(worldPos, _tetromino);
		}
	}

	void driftTetrominoBy(const sb::Vector2i& translation) {
		sb::Vector2f cellSize = getCellSize();
		sb::Vector2f worldTranslation(translation.x * cellSize.x, translation.y * cellSize.y);
		sb::Vector2f end = _tetromino.getPosition() + worldTranslation;
		driftTetrominoTo(end);
	}

	inline void driftTetrominoBy(int x, int y) { driftTetrominoBy(sb::Vector2i(x, y)); }

	void spinTetromino() {
		_tetromino.getEffects().spinBy(-90 * sb::ToRadian, _tetromino, 0.75f);

		if (isInvalid(_tetromino)) 
			_tetromino.getEffects().spinBy(90 * sb::ToRadian, _tetromino);
	}

	void popTetromino() {
		_tetromino.getEffects().pop(_tetromino);
	}

	void dropTetromino() {
		drop(_tetromino);
	}

	void quickdropTetromino() {
		Tetromino projection = computeProjection();
		_tetromino.getEffects().bounceTo(projection.getPosition(), _tetromino);
	}

	void dropBlocks() {
		for (size_t i = 0; i < _blocks.size(); i++)
			bounceBy(_blocks[i], sb::Vector2i(0, -1));
	}

	void implodeBlocks(float duration = 0.8f) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].implode(duration);
	}

	void clearLowestLineWithBlocks() {
		int y = getLowestLineWithBlocks();
		if (y >= 0)
			clearLine(y);
	}

	Tetromino computeProjection() {
		Tetromino projection = _tetromino;
		projection.getEffects().stop();
		while (!isInvalid(projection))
			moveBy(projection, sb::Vector2i(0, -1));
		moveBy(projection, sb::Vector2i(0, 1));

		return projection;
	}

	void updateEntities(float ds) {
		dispose();

		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].update(ds);

		if (_hasTetromino)
			_tetromino.update(ds);
		else
			createRandomTetromino();
	}

	void update(float ds) {

		updateEntities(ds);

		if (!_isDead) {
			step(_tetromino, ds);
		}
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();

		if (_isGridEnabled)
			target.draw(_grid, states);

		for (size_t i = 0; i < _blocks.size(); i++) 
			_batch.draw(_blocks[i], states);
		target.draw(_batch);
		
		if (_hasTetromino) {
			Tetromino projection = computeProjection();
			projection.setColor(sb::Color(1, 1, 1, 0.25f));
			_batch.draw(projection, states);
			_batch.draw(_tetromino, states);
			target.draw(_batch);
		}
	}
};

void demo14() {
	sb::Window window(400, getWindowHeight(400));
	Board board(sb::Vector2i(10, 15));

	board.createBlock('j', sb::Vector2i(5, 5));
	board.createTetromino('i');

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

sb::Vector2i getWindowSize(size_t width, float inverseAspect) {
	return sb::Vector2i(width, int(inverseAspect * width));
}

void adjustCameraToBoard(sb::Camera& camera, const Board& board) {
	float cameraAspect = camera.getAspectRatio();
	const sb::Vector2f& boardSize = board.getSize();
	float boardAspect = boardSize.x / boardSize.y;

	if (boardAspect > cameraAspect)
		camera.setWidth(board.getSize().x);
	else
		camera.setHeight(board.getSize().y);	
}

void demo15() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);
	board.createBlock('j', sb::Vector2i(5, 5));
	board.createTetromino('i');

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo16() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);
	board.createBlock('j', sb::Vector2i(5, 10));
	board.createTetromino('i');

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo17() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 5));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo18() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Grid grid(sb::Vector2i(2, 2), 0.01f);
	Tetromino tetromino('m');

	tetromino.setPosition(-0.25f, 0.25f);
	tetromino.setScale(0.5f);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r))
			tetromino.rotate(-90 * sb::ToRadian);
		const std::vector<sb::Vector2f> blockPositions = tetromino.getBlockPositions();
		std::cout << blockPositions[1].x << " " << blockPositions[1].y << std::endl;
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(grid);
		window.draw(tetromino);
		window.display();
	}
}

bool isTetrominoTouchGoingDown(sb::Window&window, Board& board) {
	if (board.hasTetromino())
		return isTouchGoingDown(window, board.getTetromino());

	return false;
}

void demo19() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);
	board.createTetromino('i', sb::Vector2i(4, 2));
	board.createBlock('j', sb::Vector2i(2, 2));

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::r) || 
			isTetrominoTouchGoingDown(window, board))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void input20(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::r) ||
		isTetrominoTouchGoingDown(window, board))
		board.spinTetromino();
}

void demo20() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input20(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void addBlocks(Board& board) {
	for (int i = 0; i < board.getBoardSize().y; i++)
		board.createBlock('m', sb::Vector2i(1, i));
}

bool isTouchDown(sb::Window& window, Board& board) {
	if (sb::Input::isTouchDown(1)) 
		return board.getBounds().contains(sb::Input::getTouchPosition(window)); 
	
	return false;
}

void input21(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(-1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(0, -1);
	if (isTouchDown(window, board))
	board.driftTetrominoTo(sb::Input::getTouchPosition(window));
}

void demo21() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);
	addBlocks(board);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input21(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		drawOutline(window, board.getBounds(), 0.02f, sb::Color(0, 1, 0, 1));
		window.display();
	}
}

void input22(sb::Window& window, Board& board) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(-1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(1, 0);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(0, -1);
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
		board.spinTetromino();
	if (isTouchDown(window, board))
		board.driftTetrominoTo(sb::Input::getTouchPosition(window));
}

void demo22() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input22(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void draw23(sb::DrawTarget& target, Board& board) {
	Tetromino projection = board.computeProjection();
	drawOutline(target, projection.getBounds(), 0.01f, sb::Color(0, 1, 0, 1));
}

void input23(sb::Window& window, Board& board) {
	bool quickdrop = false;

	if (sb::Input::isTouchDown(1)) {
		Tetromino projection = board.computeProjection();
		const sb::Vector2f touch = sb::Input::getTouchPosition(window);
		if (projection.getBounds().contains(touch)) {
			board.quickdropTetromino();
			quickdrop = true;
		}
	}

	if (sb::Input::isKeyGoingDown(sb::KeyCode::Space)) {
		board.quickdropTetromino();
		quickdrop = true;
	}
		
	if (!quickdrop)
		input22(window, board);
}

void demo23() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 10));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input23(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		draw23(window, board);
		window.display();
	}
}

void demo24() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(4, 4));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('m', sb::Vector2i(0, 0));
	board.createBlock('m', sb::Vector2i(1, 0));
	board.createBlock('m', sb::Vector2i(2, 0));
	board.createBlock('m', sb::Vector2i(1, 1));
	board.createBlock('m', sb::Vector2i(0, 2));
	board.createBlock('m', sb::Vector2i(1, 2));
	board.createBlock('m', sb::Vector2i(2, 2));
	board.createBlock('m', sb::Vector2i(1, 3));
	board.createTetromino('i', sb::Vector2i(3, 2));
	board.spinTetromino();
	board.enableGrid(true);
	board.setStepInterval(1);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo25() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);
		input22(window, board);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

bool isProjectionTouchGoingDown(sb::Window& window, Board& board) {
	if (sb::Input::isTouchGoingDown(1)) {
		Tetromino projection = board.computeProjection();
		const sb::Vector2f touch = sb::Input::getTouchPosition(window);
		return projection.getBounds().contains(touch);
	}

	return false;
}

sb::Vector2f getSwipe(sb::Window& window, float ds) {
	static sb::Vector2f start;
	static float secondsSinceStart;
	static bool swiping = false;
	const float maxSwipeSeconds = 0.75f;

	bool swipeStarting = sb::Input::isTouchGoingDown(1);
	bool swipeEnding = swiping && !sb::Input::isTouchGoingDown(1) && !sb::Input::isTouchDown(1);

	if (swipeStarting) {
		start = sb::Input::getTouchPosition(window);
		swiping = true;
		secondsSinceStart = 0;
		return sb::Vector2f(0, 0);
	}
	else if (swipeEnding) {
		swiping = false;
		if (secondsSinceStart <= maxSwipeSeconds)
			return sb::Input::getTouchPosition(window) - start;
	}

	secondsSinceStart += ds;
	return sb::Vector2f(0, 0);
}

namespace {
	class Game : public sb::Drawable {
		Board _board;
		size_t _linesCleared;
		size_t _score;
		sb::Vector2f touchOffset;

	protected:
		void updateScore() {
			size_t previousLinesCleared = _linesCleared;
			_linesCleared = _board.getLinesCleared();

			size_t newLinesCleared = _linesCleared - previousLinesCleared;
			if (newLinesCleared > 0) {
				_score += (size_t)pow(10, newLinesCleared);
				SB_MESSAGE("Score: " << _score);
			}
		}

		void updateStepInterval() {
			size_t steps = _board.getLinesCleared() / 10;
			float stepInterval = std::max(0.5f - 0.1f * steps, 0.1f);
			_board.setStepInterval(stepInterval);
		}

	public:
		Game(const sb::Vector2i& boardSize) : _board(boardSize), _linesCleared(0), _score(0)
		{ }

		inline Board& getBoard() { return _board; }

		void input(sb::Window& window, float ds) {
			if (sb::Input::isTouchGoingDown(1))
				touchOffset = _board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
			if (sb::Input::isTouchDown(1))
				_board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
			if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect())
				_board.spinTetromino();
			if (isProjectionTouchGoingDown(window, _board))
				_board.quickdropTetromino();

			if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
				_board.driftTetrominoBy(-1, 0);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
				_board.driftTetrominoBy(1, 0);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
				_board.driftTetrominoBy(0, -1);
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
				_board.spinTetromino();
			if (sb::Input::isKeyGoingDown(sb::KeyCode::Space))
				_board.quickdropTetromino();
		}

		void update(float ds) {
			_board.update(ds);

			updateScore();
			updateStepInterval();
		}

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
			target.draw(_board, states);
		}
	};
}

void demo26() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Game game(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), game.getBoard());
	game.getBoard().enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		game.update(ds);
		game.input(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(game);
		window.display();
	}
}

void demo27() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Game game(sb::Vector2i(10, 18));

	adjustCameraToBoard(window.getCamera(), game.getBoard());
	game.getBoard().enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		game.update(ds);
		game.input(window, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(game);
		window.display();
	}
}

void demo28() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Quad quad;
	std::vector<sb::Tween2f> tweens(32);
	size_t currentTween = 0;
	float time = 0;

	quad.setScale(0.1f);

	tweens[0] = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0), 0.3f).wait(0.5f).circInOut(sb::Vector2f(0), sb::Vector2f(-0.3f, 0.5f), 0.2f);
	tweens[1] = sb::Tween2f().linear(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[2] = sb::Tween2f().sineIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[3] = sb::Tween2f().sineOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[4] = sb::Tween2f().sineInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[5] = sb::Tween2f().quadIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[6] = sb::Tween2f().quadOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[7] = sb::Tween2f().quadInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[8] = sb::Tween2f().cubicIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[9] = sb::Tween2f().cubicOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[10] = sb::Tween2f().cubicInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[11] = sb::Tween2f().quartIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[12] = sb::Tween2f().quartOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[13] = sb::Tween2f().quartInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[14] = sb::Tween2f().quintIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[15] = sb::Tween2f().quintOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[16] = sb::Tween2f().quintInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[17] = sb::Tween2f().expoIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[18] = sb::Tween2f().expoOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[19] = sb::Tween2f().expoInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[20] = sb::Tween2f().circIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[21] = sb::Tween2f().circOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[22] = sb::Tween2f().circInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[23] = sb::Tween2f().backIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[24] = sb::Tween2f().backOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[25] = sb::Tween2f().backInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[26] = sb::Tween2f().elasticIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[27] = sb::Tween2f().elasticOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[28] = sb::Tween2f().elasticInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[29] = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[30] = sb::Tween2f().bounceOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);
	tweens[31] = sb::Tween2f().bounceInOut(sb::Vector2f(-0.4f), sb::Vector2f(0.4f), 1);

	while (window.isOpen()) {
		time += getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.setPosition(tweens[currentTween].value(time));
		if (sb::Input::isTouchGoingDown(1)) {
			currentTween = (currentTween + 1) % tweens.size();
			time = 0;
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);

		window.display();
	}
}

void demo29() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	sb::Quad quad;
	Animation2f animation;

	quad.setScale(0.1f);
	animation.tween = sb::Tween2f().bounceIn(sb::Vector2f(-0.4f), sb::Vector2f(0), 0.3f).wait(0.5f).circInOut(sb::Vector2f(0), sb::Vector2f(-0.3f, 0.5f), 0.2f);	
	
	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		if (sb::Input::isTouchGoingDown(1) && !animation.isPlaying())
			animation.start();
		std::cout << (animation.isPlaying() ? "playing" : "not playing") << std::endl;

		animation.update(ds);
		quad.setPosition(animation.value());

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);

		window.display();
	}
}

class MyQuad : public sb::Drawable, public sb::Transformable {
	sb::Quad _quad;
	TransformEffects2 _effects;

public:
	MyQuad() : _effects(*this)
	{ }

	void update(float ds) {
		_effects.update(ds);
	}

	inline TransformEffects2& getEffects() { return _effects; }

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();
		target.draw(_quad, states);
	}
};

void demo30() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	MyQuad quad;

	quad.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			quad.getEffects().pop();
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void demo31() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	MyQuad quad;

	quad.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		quad.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			quad.getEffects().driftOld(touch);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(quad);
		window.display();
	}
}

void print32(TransformEffects2& effects) {
	if (effects.isMoving())
		std::cout << "driftOld: " << effects.getTargetPosition().x << " " << effects.getTargetPosition().y << std::endl;

	if (effects.isRotating())
	std::cout << "spinOld: " << effects.getTargetRotation() * sb::ToDegrees << std::endl;

	if (effects.isScaling())
		std::cout << "pop: " << effects.getTargetScale().x << std::endl;
}

sb::Vector2f discretize(const sb::Vector2f& v, const sb::Vector2f& discretization) {
	float x = floor(v.x / discretization.x);
	float y = floor(v.y / discretization.y);
	return sb::Vector2f(x * discretization.x, y * discretization.y) + 0.5f * discretization;
}

void input32(sb::Window& window, TransformEffects2& effects) {
	const sb::Vector2f cellSize = sb::Vector2f(0.1f, 0.1f);

	if (sb::Input::isTouchDown(1)) {
		sb::Vector2f touch = sb::Input::getTouchPosition(window);
		effects.driftOld(discretize(touch, cellSize));
	}
	if (sb::Input::isTouchGoingDown(1))
		effects.pop();

	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left)) {
		sb::Vector2f pos = effects.getTargetPosition();
		effects.driftOld(discretize(pos - sb::Vector2f(cellSize.x, 0), cellSize));
	}
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right)) {
		sb::Vector2f pos = effects.getTargetPosition();
		effects.driftOld(discretize(pos + sb::Vector2f(cellSize.x, 0), cellSize));
	}
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
		float angle = effects.getTargetRotation();
		effects.spinOld(angle - 90 * sb::ToRadian);
	}
}

void demo32() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Tetromino tetromino('z');
	TransformEffects2 effects(tetromino);

	tetromino.setScale(0.1f);
	tetromino.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		effects.update(ds);
		input32(window, effects);
		print32(effects);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo33() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('z');
	TransformEffects2 effects(block);
	
	srand(1234567891);

	block.setScale(0.1f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		effects.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			effects.die(1.1f);
		if (sb::Input::isKeyDown(sb::KeyCode::b)) {
			sb::Vector2f discretization(0.1f, 0.1f);
			sb::Vector2f discretized = discretize(block.getPosition(), discretization);
			effects.bounce(discretized - sb::Vector2f(0, discretization.y), 0.5f);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo34() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			tetromino.getEffects2().pop();
		if (sb::Input::isTouchDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			tetromino.getEffects2().driftOld(touch);
		}
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects2().spinOld(tetromino.getRotation() - 90 * sb::ToRadian);
			
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo35() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('z');

	block.setScale(0.1f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			block.getEffects2().bounce(block.getPosition() - sb::Vector2f(0, 0.1f));

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			block.getEffects2().die();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo36() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('z');

	block.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			block.getEffects2().driftTo(touch, 1);
		}
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo37() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1)) {
			sb::Vector2f touch = sb::Input::getTouchPosition(window);
			tetromino.getEffects2().driftTo(touch, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo38() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(2, 2));

	board.enableGrid(true);
	board.createTetromino('m', sb::Vector2i(0, 1));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Down)) {
			board.driftTetrominoBy(sb::Vector2i(0, -1));
		}
		
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo39() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('m');

	tetromino.setScale(0.2f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			tetromino.getEffects2().spinBy(45 * sb::ToRadian, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo40() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Block block('m');

	block.setScale(0.2f);
	Block block2 = block;

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block2.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			block2.getEffects().spinBy(90 * sb::ToRadian, block2, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block2);
		window.display();
	}
}

void demo41() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('z');

	tetromino.setScale(0.2f);
	tetromino.setPosition(0.2f, 0.3f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up)) {
			tetromino.getEffects().spinBy(90 * sb::ToRadian, tetromino, 1);
		}

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void demo42() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(4, 4));

	board.enableGrid(true);
	board.createTetromino('m', sb::Vector2i(1, 2));

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			board.spinTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo43() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Tetromino tetromino('m');

	tetromino.setScale(0.1f);
	tetromino.setPosition(0.2f);
	tetromino.setRotation(45 * sb::ToRadian);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		tetromino.update(ds);
		if (sb::Input::isTouchGoingDown(1))
			tetromino.getEffects().driftTo(sb::Input::getTouchPosition(window), tetromino, 2);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
			tetromino.getEffects().spinBy(-90 * sb::ToRadian, tetromino);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
			tetromino.getEffects().pop(tetromino, 1);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(tetromino);
		window.display();
	}
}

void touchInput44(sb::Window& window, Board& board, float ds) {
	static float secondsSinceLastTouch = 1;
	static sb::Vector2f touchOffset;
	secondsSinceLastTouch += ds;

	if (sb::Input::isTouchGoingDown(1)) {
		touchOffset = board.getTetromino().getPosition() - sb::Input::getTouchPosition(window);
		if (secondsSinceLastTouch >= 1) 
			board.popTetromino();
		secondsSinceLastTouch = 0;
	}
	if (sb::Input::isTouchDown(1))
		board.driftTetrominoTo(sb::Input::getTouchPosition(window) + touchOffset);
	if (getSwipe(window, ds).y > 0.05f * window.getInverseAspect() 
		|| isTouchGoingDown(window, board.getTetromino()))
		board.spinTetromino();
}

void input44(sb::Window& window, Board& board, float ds) {
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Up))
		board.spinTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::p))
		board.popTetromino();
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Down))
		board.driftTetrominoBy(sb::Vector2i(0, -1));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Left))
		board.driftTetrominoBy(sb::Vector2i(-1, 0));
	if (sb::Input::isKeyGoingDown(sb::KeyCode::Right))
		board.driftTetrominoBy(sb::Vector2i(1, 0));

	touchInput44(window, board, ds);
}

void demo44() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('m', sb::Vector2i(4, 4));
	board.createBlock('j', sb::Vector2i(4, 2));
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.getTetromino().update(ds);
		input44(window, board, ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo45() {
	srand(12353859);
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('j');

	block.setScale(0.5f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::i))
			block.implode();

		std::cout << (int)block.getState() << std::endl;

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo46() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('m', sb::Vector2i(4, 4));
	board.createBlock('m', sb::Vector2i(5, 5));
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::i)) 
			board.implodeBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo47() {
	srand(12353859);
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Light light;
	Block block('j');

	block.setScale(0.2f);
	block.setPosition(0, 0.3f);
	block.setLight(light);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		block.update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			block.getEffects().bounceBy(sb::Vector2f(0, -0.1f), block, 0.75f);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(block);
		window.display();
	}
}

void demo48() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 8));
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		for (size_t i = 0; i < board.getBlocks().size(); i++)
			board.getBlocks()[i].update(ds);
		if (board.hasTetromino())
			board.getTetromino().update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b) && board.hasTetromino())
			board.dropTetromino();
	
		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo49() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createBlock('z', sb::Vector2i(4, 8));
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		for (size_t i = 0; i < board.getBlocks().size(); i++)
			board.getBlocks()[i].update(ds);
		if (board.hasTetromino())
			board.getTetromino().update(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::b))
			board.dropBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo50() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();

		if (sb::Input::isKeyGoingDown(sb::KeyCode::q))
			board.quickdropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void fillLine(Board& board, int y) {
	int width = board.getBoardSize().x;
	for (int x = 0; x < width; x++)
		board.createBlock('z', sb::Vector2i(x, y));
}

void demo51() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	fillLine(board, 0);
	fillLine(board, 1);
	fillLine(board, 2);

	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c))
			board.clearLowestLineWithBlocks();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void addBlocks52(Board& board, int y, std::vector<int> horizontalPositions) {
	std::vector<sb::Vector2i> positions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0), sb::Vector2i(3, 0),
		sb::Vector2i(6, 0), sb::Vector2i(7, 0), sb::Vector2i(8, 0), sb::Vector2i(9, 0),
		sb::Vector2i(0, 1), sb::Vector2i(1, 1), sb::Vector2i(2, 1), sb::Vector2i(3, 1),
		sb::Vector2i(6, 1), sb::Vector2i(7, 1), sb::Vector2i(8, 1), sb::Vector2i(9, 1),
		sb::Vector2i(0, 2), sb::Vector2i(1, 2) };

	for (size_t i = 0; i < horizontalPositions.size(); i++)
		board.createBlock('j', sb::Vector2i(horizontalPositions[i], y));
}

void addBlocks52(Board& board) {
	addBlocks52(board, 0, { 0, 1, 2, 3, 6, 7, 8, 9 });
	addBlocks52(board, 1, { 0, 1, 2, 3, 6, 7, 8, 9 });
	addBlocks52(board, 2, { 1, 2 });
}

void demo52() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 4));
	addBlocks52(board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo53() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	Board board(sb::Vector2i(10, 14));

	adjustCameraToBoard(window.getCamera(), board);
	board.createTetromino('z', sb::Vector2i(4, 4));
	addBlocks52(board);
	board.enableGrid(true);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		board.updateEntities(ds);

		input44(window, board, ds);
		if (sb::Input::isKeyGoingDown(sb::KeyCode::d))
			board.dropTetromino();
		if (sb::Input::isKeyGoingDown(sb::KeyCode::q))
			board.quickdropTetromino();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

sb::Color createColor(int r, int g = 255, int b = 255, int a = 255) {
	return sb::Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void initColors54(std::vector<sb::Color>& colors) {
	colors.resize(7);
	int alpha = 150;
	colors[0] = createColor(255, 242,   0, alpha);
	colors[1] = createColor(  0, 240, 240, alpha);
	colors[2] = createColor(  0,   0, 240, alpha);
	colors[3] = createColor(  0, 240,   0, alpha);
	colors[4] = createColor(240, 160,   0, alpha);
	colors[5] = createColor(240,   0,   0, alpha);
	colors[6] = createColor(160,   0, 240, alpha);
}

void demo54() {
	sb::Window window(getWindowSize(400, 3.f / 2.f));
	std::vector<sb::Color> colors;
	std::size_t currentColor = 0;
	sb::Texture texture;
	sb::ParticleSystem particleSystem(1024);

	initColors54(colors);
	texture.loadFromAsset("Textures/SimpleParticle.png");
	texture.enableMipmap(true);
	particleSystem.setTexture(texture);
	particleSystem.setEmissionRatePerSecond(50);
	particleSystem.setParticleSizeRange(sb::Vector2f(0.1f, 0.13f));
	particleSystem.setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
	particleSystem.setParticleColor(createColor(255, 242, 0, 150));
	particleSystem.setEmissionShape(sb::Disk(0.075f, 0.1f, 0, 2 * sb::Pi));
	particleSystem.setScale(0.3f);

	while (window.isOpen()) {
		float ds = getDeltaSeconds();
		sb::Input::update();
		window.update();
		particleSystem.update(ds);

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(particleSystem);

		if (sb::Input::isKeyGoingDown(sb::KeyCode::c)) {
			currentColor = (currentColor + 1) % colors.size();
			particleSystem.setParticleColor(colors[currentColor]);
		}

		//window.draw(board);
		window.display();
	}
}

void demo() {
	demo54();

	//demo53();
	
	//demo52();

	//demo51();

	//demo49();

	//demo48();

	//demo47();

	//demo46();

	//demo45();

	//demo44();

	//demo43();

	//demo42();

	//demo41();

	//demo40();

	//demo39();

	//demo38();

	//demo37();

	//demo36();

	//demo35();

	//demo34();

	//demo33();

	//demo32();

	//demo31();

	//demo30();

	//demo29();

	//demo28();

	//demo27();

	//demo26();
	
	//demo25();

	//demo24();

	//demo23();

	//demo22();

	//demo21();

	//demo20();

	//demo19();

	//demo18();

	//demo17();

	//demo16();

	//demo15();

	//demo14();

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

    //demo0();
}
