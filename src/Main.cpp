#include "Game.h"
#include "Logger.h"
#include "CameraDemo1.h"

// TODO: 
// - Draft Tetromino
//   - Fix Android segfault upon first start after install
//   - Collision
//   - Rotation
//   - Autodrop
//   - Hard Drop
//   - Annealing
//   - Respawn

int main() {
    blocks::runGame();

    return 0;
}
