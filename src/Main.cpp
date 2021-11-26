#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h" 
#include "TweenDemo1.h"

// TODO:
// use https://cplusplus.com/reference/cstdlib/wcstombs/?kw=wcstombs in Asset.cpp to fix the conversion warning
// create final tween implementation file with demo


using namespace blocks;

int main() {
    tweenDemo1::run();
    //blocks::runGame();

    return 0;
}