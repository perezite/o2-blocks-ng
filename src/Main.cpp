#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h"
#include "TweenDemo1.h"

// TODO: 
// - Animation concept
// - Define tween interface including chaining
// - Tween draft (in Demos), must have:
//   - tweening functions OK
//   - duration OK
//   - chaining
//   - replace existing tweens by new ones. also make compatible with particle systems

using namespace blocks;

int main() {
    tweenDemo1::run();
    //blocks::runGame();

    return 0;
}
