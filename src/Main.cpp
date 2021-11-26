#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h" 
#include "TweenDemo1.h"

// TODO:
// - Define tween interface
// - Tween draft (in Demos), must have:
//   - make compatible with particle systems
//   - replace existing tweens by new ones


using namespace blocks;

int main() {
    tweenDemo1::run();
    //blocks::runGame();

    return 0;
}