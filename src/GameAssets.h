#pragma once

#include "Texture.h"
#include <string>

namespace blocks
{
    struct GameAssets 
    {
        GameAssets();

        sb::Texture blockTextures;

        sb::Texture squareTextures;
    };
}
