#pragma once
#include "dev_util.hpp"
#include "draw_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include <unordered_set>
#include <vector>

class MiniMap {
public:
  int width;
  int height;
  TileManager *tManager;
  MiniMap(int init_width, int init_height, TileManager *tileManager);
  ~MiniMap();

  Camera2D camera = {0};
  Rectangle position; // the position that the minimap is on the screen

  float defaultZoom = 1.0f;
  float fullScreenZoom = 2.9f;

  // two different zooms depending on minimap mode
  Vector2 fullScreenTarget;
  Vector2 fullScreenOffset;

  RenderTexture map; // the RenderTexture of the minimap

  float padding =
      10.0f; // the space from the minimap window and the edge of the screen

  unsigned char alphaValue = 254; // the transparent value of each tile

  float tileSize = 3.0f;   // the dimensions of each tile in the minimap
  bool fullScreen = false; // large minimap mode toggle usage
  int visitedChunks[WORLD_SIZE * 2 * WORLD_SIZE *
                    2]; // the chunks that the player has visited, 0 or 1
  RenderTexture mapChunks[WORLD_SIZE * 2 * WORLD_SIZE *
                          2]; // RenderTexture of each chunk in the minimap

  std::unordered_set<Vector2, Vector2Util, Vector2Util>
      waypoints; // waypoints the player has placed on the minimap, only one for
                 // each tile

  void updateChunks();
  void storeChunkTexture(int index);
  void storeNewChunkTextures(Vector2 playerPos);
  void getMapChangeInput(UI &ui, Vector2 playerPos);
  void createWaypoint();
  void deleteWaypoint();
  void drawMapChunks(Vector2 playerPos, Texture2D &medium);
  void drawVisitedChunks(std::vector<Vector2> &chunkBuffer);
  void draw(Vector2 playerPos, SpriteDrawSystem &drawSystem);
  void update(UI &ui, Vector2 playerPos, SpriteDrawSystem &drawSystem);
};
