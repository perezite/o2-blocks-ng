#include "Window.h"
#include "DrawBatch.h"
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

inline float getDesktopAspectRatio() { return 16.0f / 9.0f; }

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
		float x = (gridPos.x - 1) * scale.x;
		float y = (gridPos.y - 1) * scale.y;
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
	sb::Window window(400, int(400 * getDesktopAspectRatio()));
	sb::DrawBatch batch(512);
	sb::Texture stoneTex;
	std::vector<sb::Sprite> stones(7);

	stoneTex.loadFromAsset("Textures/Stones.png");
	init0(stones, stoneTex);

	while (window.isOpen()) {
		sb::Input::update();
		window.update();
 
		window.clear(sb::Color(1, 1, 1, 1));
		draw0(batch, stones);
		window.draw(batch);
		window.display();
	}
}

int main() {
	version();

	demo0();

	return 0;
}
