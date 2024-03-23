#pragma once
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <vector>

class MiniMap {
public:
  Camera2D camera = {0};
  float defaultZoom = 1.0f;
  float fullScreenZoom = 2.9f;
  Vector2 fullScreenTarget;
  Vector2 fullScreenOffset;

  RenderTexture map;

  float padding = 10.0f;
  unsigned char alphaValue = 255;
  unsigned char fullScreenAlphaValue = 222;

  float fullScreenTileSize = 3.0f;
  float defaultTileSize = 0.8f;

  bool fullScreen = false;
  bool updateMap = true;
  Rectangle position;

  int visitedChunks[WORLD_SIZE * 2 * WORLD_SIZE * 2];
  RenderTexture mapChunks[WORLD_SIZE * 2 * WORLD_SIZE * 2];

  int width;
  int height;
  MiniMap(int init_width, int init_height)
      : width(init_width), height(init_height) {
    map = LoadRenderTexture(init_width, init_height);
    position = Rectangle{GetScreenWidth() - width - padding, padding,
                         (float)init_width, (float)init_height};
  };
  ~MiniMap();

  void updateChunks(TileManager &tileManager);
  void storeChunkTexture(TileManager &tileManager, int index);
  void storeNewChunkTextures(TileManager &tileManager, Vector2 playerPos);
  void getMapChangeInput(UI &ui, Vector2 playerPos);

  void drawMapChunks(TileManager &tileManager, Vector2 playerPos,
                     Texture2D &medium);

  void drawNearChunks(TileManager &tileManager,
                      std::vector<Vector2> &chunkBuffer);
  void drawVisitedChunks(TileManager &tileManager,
                         std::vector<Vector2> &chunkBuffer);

  void draw(TileManager &tileManager, Vector2 playerPos, Texture2D &medium);

  void update(TileManager &tileManager, UI &ui, Vector2 playerPos,
              Texture2D &medium);
};
