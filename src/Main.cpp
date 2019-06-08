#include "Window.h"
#include "DrawBatch.h"
#include "Quad.h"
#include "Input.h"
#include "Stopwatch.h"
#include "TextureSheet.h"
#include "Sprite.h"
#include <iostream>

float getSeconds() {
	static sb::Stopwatch sw;
	return sw.getElapsedSeconds();
}

float getDeltaSeconds()
{
	static float lastElapsed = 0;
	float elapsed = getSeconds();
	float delta = elapsed - lastElapsed;
	lastElapsed = elapsed;
	return delta;
}

void version() {
	#ifdef _DEBUG
		std::string configuration = "Debug";
	#else
		std::string configuration = "Release";
	#endif	
	SB_MESSAGE("Blocks (" << configuration << "): " << __DATE__ << ", " << __TIME__);
}

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

int main() {
	version();

	demo3();

	//demo2();

	//demo1();

	// demo0();

	return 0;
}
