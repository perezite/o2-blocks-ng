#include "Texture.h"
#include <string>

namespace blocks
{
    class AssetStore 
    {
        static AssetStore* Instance;

        static bool IsInit;

        sb::Texture _blockTextureSheet;

    public:
        static AssetStore& getInstance();

        static void init();

        static void release();

        sb::Texture& getBlockTextureSheet() { return _blockTextureSheet; }

    protected:
        AssetStore();
    };
}
