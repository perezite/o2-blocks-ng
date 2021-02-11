#include "AssetStore.h"

using namespace std;

namespace blocks
{
    AssetStore* AssetStore::Instance = NULL;

    bool AssetStore::IsInit = false;

    AssetStore& AssetStore::getInstance()
    {
        if (!IsInit)
            SB_ERROR("Asset store must be initialized using init() before using it");

        return *Instance;
    }

    void AssetStore::init()
    {
        if (!Instance)
            Instance = new AssetStore();

        IsInit = true;
    }

    void AssetStore::release()
    {
        if (Instance)
            delete Instance;

        IsInit = false;
    }

    AssetStore::AssetStore() :
        _blockTextureSheet("Textures/Blocks.png")
    { }
}
