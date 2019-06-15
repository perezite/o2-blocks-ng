#include "Window.h"
#include "DrawBatch.h"
#include "Triangle.h"
#include "Quad.h"
#include "Input.h"
#include "Stopwatch.h"
#include "TextureSheet.h"
#include "Sprite.h"
#include "Math.h"
#include <cstddef>
#include <vector>
#include <map>

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

void init1(sb::Mesh& background, sb::Camera& camera) {
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

class Light {
    sb::Vector2f _direction;

public: 
    Light(const sb::Vector2f& direction = sb::Vector2f(0, -1)) : _direction(direction)
    { }

    inline void setDirection(const sb::Vector2f& direction) { _direction = direction; }

    inline const sb::Vector2f& getDirection() const { return _direction; }
};

class Block : public sb::Drawable, public sb::Transformable {
    sb::Sprite _sprite;
    const Light* _light;

protected:
    static sb::Texture& getSheet() {
        static sb::Texture texture("Textures/Blocks.png");
        return texture;
    }

    inline const sb::IntRect getTextureArea(std::size_t row, std::size_t col) {
        static sb::Vector2i blockSize(128, 128);
        return sb::IntRect(col * blockSize.x, row * blockSize.y, blockSize.x, blockSize.y);
    }

    void setTexture(char type) {
        if (type == 'i')
            _sprite.setTexture(getSheet(), getTextureArea(2, 1));
        else if (type == 'j')
            _sprite.setTexture(getSheet(), getTextureArea(2, 2));
        else if (type == 'm')
            _sprite.setTexture(getSheet(), getTextureArea(2, 1));
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

public:
    Block(char type = 'i') : _light(NULL) 
    {
        setType(type);
    }

    void setType(char type) {
        type = tolower(type);
        setTexture(type);
    }

    inline void setLight(const Light& light) { _light = &light; }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
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

class Tetromino : public sb::Drawable, public sb::Transformable {
    std::vector<Block> _blocks;
    std::vector<sb::Vector2i> _blockPositions;
    sb::IntRect _blockBounds;

protected:
    void clear() {
        _blocks.clear();
        _blockPositions.clear();	
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
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(0, -1), sb::Vector2i(1, -1) };
        else if (type == 'm')
            _blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0) };

        createBlocks(_blockPositions, type);
    }

    sb::FloatRect getTransformedBounds(sb::FloatRect& bounds) {
        std::vector<sb::Vector2f> edges(4);
        edges[0] = sb::Vector2f(bounds.left, bounds.bottom);
        edges[1] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom);
        edges[2] = sb::Vector2f(bounds.left, bounds.bottom + bounds.height);
        edges[3] = sb::Vector2f(bounds.left + bounds.width, bounds.bottom + bounds.height);
        for (size_t i = 0; i < edges.size(); i++) 
            edges[i] *= getTransform();

        sb::Vector2f min, max;
        computeExtents(edges, min, max);
        return sb::FloatRect(min.x, min.y, max.x - min.x, max.y - min.y);
    }

public:
    Tetromino(char type = 'i') {
        setType(type);
    }

    void setType(char type) {
        clear();
        createBlocks(tolower(type));
    }

    inline void setLight(Light& light) { 
        for (std::size_t i = 0; i < _blocks.size(); i++)
            _blocks[i].setLight(light);
    }

    sb::FloatRect getBounds() {
        sb::Vector2f blockSize(1.f, 1.f);
        sb::FloatRect bounds(_blockBounds.left - 0.5f, _blockBounds.bottom - 0.5f, (float)_blockBounds.width, (float)_blockBounds.height);

		return getTransformedBounds(bounds);
    }

    virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
        states.transform *= getTransform();
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
	sb::Vector2i _boardSize;
	Grid _grid;
	std::vector<Block> _blocks;

protected:
	sb::Vector2f boardToWorldPosition(const sb::Vector2i& boardPos) {
		sb::Vector2f size(1, _boardSize.y / float(_boardSize.x));
		sb::Vector2f halfSize = 0.5f * size;
		sb::Vector2f delta(size.x / _boardSize.x, size.y / _boardSize.y);
		return sb::Vector2f(-halfSize.x + (boardPos.x - 0.5f) * delta.x,
			-halfSize.y + (boardPos.y - 0.5f) * delta.y);
	}

	sb::Vector2f getCellSize() {
		return sb::Vector2f(1.f / _boardSize.x, 1.f / _boardSize.x);
	}

public:
	Board(const sb::Vector2i& boardSize) : _boardSize(boardSize), _grid(boardSize, 0.005f)
	{ }

	void createBlock(char type, const sb::Vector2i& position) {
		Block block(type);
		block.setPosition(boardToWorldPosition(position));
		block.setScale(getCellSize());
		_blocks.push_back(block);
	}

	virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault()) {
		states.transform *= getTransform();

		target.draw(_grid, states);

		for (size_t i = 0; i < _blocks.size(); i++) {
			target.draw(_blocks[i], states);
		}
	}

};

void demo14() {
	sb::Window window(400, getWindowHeight(400));
	Board board(sb::Vector2i(10, 15));

	 board.createBlock('j', sb::Vector2i(5, 5));
	// board.createTetromino('i');

	while (window.isOpen()) {
		sb::Input::update();
		window.update();

		window.clear(sb::Color(1, 1, 1, 1));
		window.draw(board);
		window.display();
	}
}

void demo() {
	demo14();

	demo13();

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

    // demo0();
}