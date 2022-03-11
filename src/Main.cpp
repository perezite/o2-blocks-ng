#include "Scene.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h"
#include "TweenDemo1.h"

// TODO
// Simplify collision detection

using namespace blocks;

void runGame() {
	//Scene scene(4, 4);
	Scene scene(10, 18);
	scene.run();
}

int main() {
	//tweenDemo1::run();
	runGame();

	return 0;
}