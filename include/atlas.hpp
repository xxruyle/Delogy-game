#pragma once
#include "raylib.h"

class Atlas {
  public:
    int width;
    int height;
    Texture2D texture;
    Atlas(const char *filePath, int spriteWidth, int spriteHeight);
    ~Atlas();
};
