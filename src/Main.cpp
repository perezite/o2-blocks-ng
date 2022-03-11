#include "Scene.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h"
#include "TweenDemo1.h"

// TODO
// Make simple board a one liner by creating function: Scene::enableSimpleMode(x, y)
//   Shrinks the board
//   Creates a simple tetromino at the right position
//   Creates a simple block at the right position
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