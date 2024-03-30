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
  unsigned char alphaValue = 254;

  float defaultTileSize = 0.8f;
  float tileSize = 3.0f;

  bool fullScreen = false;
  bool updateMap = true;
  Rectangle position;

  int visitedChunks[WORLD_SIZE * 2 * WORLD_SIZE * 2];
  RenderTexture mapChunks[WORLD_SIZE * 2 * WORLD_SIZE * 2];

  std::vector<Vector2> waypoints;

  int width;
  int height;
  TileManager *tManager;
  MiniMap(int init_width, int init_height, TileManager *tileManager)
      : width(init_width), height(init_height) {
    map = LoadRenderTexture(init_width, init_height);
    position = Rectangle{GetScreenWidth() - width - padding, padding,
                         (float)init_width, (float)init_height};
    tManager = tileManager;
  };
  ~MiniMap();

  void updateChunks();
  void storeChunkTexture(int index);
  void storeNewChunkTextures(Vector2 playerPos);
  void getMapChangeInput(UI &ui, Vector2 playerPos);
  void playerPlaceWaypoint();

  void drawMapChunks(Vector2 playerPos, Texture2D &medium);

  void drawVisitedChunks(std::vector<Vector2> &chunkBuffer);

  void draw(Vector2 playerPos, Texture2D &medium);

  void update(UI &ui, Vector2 playerPos, Texture2D &medium);
};
