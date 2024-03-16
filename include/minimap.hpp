#pragma once
#include "raylib.h"
#include "tile_manager.hpp"
#include <vector>

class MiniMap {
public:
  struct MapCell {
    int id;
    int item;
    int screenX;
    int screenY;
    Vector2 gridPos;
  };

  /* Vector2 offset; // Camera offset (displacement from target) */
  /* Vector2 target; // Camera target (rotation and zoom origin) */
  /* float rotation; // Camera rotation in degrees */
  /* float zoom;     // Camera zoom (scaling), should be 1.0f by default */

  Camera2D camera = {0};

  RenderTexture map;

  int width;
  int height;
  int mapRadius =
      60; // the radius that the map reaches around the point of interest
  float padding = 10.0f;
  bool fullScreen = false;
  bool updateMap = true;
  Rectangle position;

  std::vector<MapCell> cellInfo;

  MiniMap(int init_width, int init_height)
      : width(init_width), height(init_height) {
    map = LoadRenderTexture(init_width, init_height);
    position = Rectangle{GetScreenWidth() - width - padding, padding,
                         (float)init_width, (float)init_height};
  };

  void populateMap(TileManager &tileManager, Vector2 playerPos);
  void check_map_update(Vector2 playerPos);
  void draw(TileManager &tileManager, UI &ui, Vector2 playerPos,
            Texture2D &medium);
  void drawMinimap(Vector2 playerPos, Texture2D &medium);

  void update(TileManager &tileManager, UI &ui, Vector2 playerPos,
              Texture2D &medium);
};
