#pragma once
#include "raylib.h"
#include "tile_manager.hpp"

class MiniMap {
public:
  int width;
  int height;
  bool fullScreen = false;
  Rectangle position;
  MiniMap(int init_width, int init_height)
      : width(init_width), height(init_height){};

  void obtainArea(Vector2 playerPos, TileManager &tileManager);
  void draw();
  void update();
};
