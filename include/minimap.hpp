#pragma once
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <vector>

class MiniMap {
public:
  Camera2D camera = {0};

  RenderTexture map;

  float padding = 10.0f;
  bool fullScreen = false;
  bool updateMap = true;
  Rectangle position;

  int visitedChunks[WORLD_SIZE * 2 * WORLD_SIZE * 2];

  int width;
  int height;
  MiniMap(int init_width, int init_height)
      : width(init_width), height(init_height) {
    map = LoadRenderTexture(init_width, init_height);
    position = Rectangle{GetScreenWidth() - width - padding, padding,
                         (float)init_width, (float)init_height};
  };

  void getMapChangeInput();

  void drawMapChunks(TileManager &tileManager, Vector2 playerPos,
                     Texture2D &medium);

  void update(TileManager &tileManager, UI &ui, Vector2 playerPos,
              Texture2D &medium);
};
