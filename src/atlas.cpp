#include "atlas.hpp"
#include "macros_util.hpp"

Atlas::Atlas(const char *filePath)
{
    assert(FileExists(filePath));

    std::cout << "ATLAS: Loading Atlas from: [" << filePath << "]" << std::endl;
    texture = LoadTexture(filePath);

    assert(texture.id > 0);
}

Atlas::~Atlas() { UnloadTexture(texture); }
