#pragma once
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <optional>
#include <vector>

/* Generally used for terrain  */
struct Tile {
  int id;
  /* default_atlas.png positions */
  int x;
  int y;
};

enum TileType {
  NULL_TILE,
  WALL_FRONT,
  CAVE_FLOOR_MIDDLE,
  DIRT_FLOOR_MIDDLE,
};

struct Item {
  int id;
  int x;
  int y;
};

enum ItemType {
  NULL_ITEM,
  RAIL_NW, // top left rail 1
  RAIL_V,  // rail vertical
  RAIL_NE, // top right rail
  RAIL_H,  // rail horizontal
  RAIL_SE, // bottom right rail
  RAIL_SW, // bottom left rail 6
  STORAGE_BOX,
  CART, // they have underscores because they are items ;)
  MUSHROOM_PURPLE
};

#define NUM_TILES 31 // the number of tiles so far
#define NUM_ITEMS 50

namespace AtlasData {
extern Tile tileids[NUM_TILES];
extern Item itemids[NUM_ITEMS];
void init();
} // namespace AtlasData
