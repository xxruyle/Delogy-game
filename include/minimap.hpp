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
  int width;
  int height;
  int mapRadius =
      60; // the radius that the map reaches around the point of interest
  bool fullScreen = false;
  bool updateMap = true;
  Rectangle position;

  std::vector<MapCell> cellInfo;

  MiniMap(int init_width, int init_height)
      : width(init_width), height(init_height){};

  void populateMap(TileManager &tileManager, Vector2 playerPos);
  void draw(TileManager &tileManager, Vector2 playerPos);
  void drawMinimap(Vector2 playerPos);
  void drawFullscreenMinimap(Vector2 playerPos);

  bool inMinimap(Vector2 ePosition,
                 Vector2 mapOrigin); // check if ePosition vector is in
                                     // the minimap coordinate radius
  void update();
};
