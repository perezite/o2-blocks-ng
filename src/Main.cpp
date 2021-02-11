#include "Game.h"
#include "CameraDemo1.h"
#include "AssetStore.h"

// TODO: Think of a better way to handle assets (pass the asset store in the constructor)
void run()
{
    //cameraDemo1::run();
    blocks::runGame();
}

int main() {
    blocks::AssetStore::init();
    run();
    blocks::AssetStore::release();

    return 0;
}
