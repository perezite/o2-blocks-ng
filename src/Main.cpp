#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h"
#include "TweenDemo1.h"

// TODO: 
// - Define tween interface
// - Tween draft (in Demos), must have:
//   - tweening functions OK
//   - duration OK
//   - chaining OK
//   - replace existing tweens by new ones. also make compatible with particle systems
// - Automate draw() calls

using namespace blocks;

int main() {
    tweenDemo1::run();
    //blocks::runGame();

    return 0;
}
