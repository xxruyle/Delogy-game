#pragma once
#include "raylib.h"

class Atlas {
  public:
    Texture2D texture;
    Atlas(const char *filePath);
    ~Atlas();
};
