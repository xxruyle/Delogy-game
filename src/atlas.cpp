#include "atlas.hpp"
#include "macros_util.hpp"
#include "raylib.h"

Atlas::Atlas(const char *filePath)
{
    assert(FileExists(filePath));

    std::cout << "ATLAS: Loading Atlas from: [" << filePath << "]" << std::endl;
    texture = LoadTexture(filePath);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    assert(texture.id > 0);
}

Atlas::~Atlas() { UnloadTexture(texture); }
