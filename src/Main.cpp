#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"
#include "SceneDemo1.h"

// TODO: 
// - Draft Tetromino
//   - Optimize performance in BlockyCollider (remove recomputation of globalPositions every frame, instead create getGlobalPositions() method with dirty flag)
//   - Autodrop
//   - Hard Drop
//   - Annealing
//   - Respawn

using namespace blocks;

int main() {
    blocks::runGame();

    return 0;
}
