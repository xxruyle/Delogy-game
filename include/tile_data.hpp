#ifndef TILE_DATA_H
#define TILE_DATA_H
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <vector>

/* Generally used for terrain  */
struct Tile {
  int id;
  /* default_atlas.png positions */
  int x;
  int y;
};

enum TileType {
  WALL_FRONT,
  CAVE_FLOOR_MIDDLE,
  DIRT_FLOOR_MIDDLE,
};

#define NUM_TILES 31 // the number of tiles so far

/* Dungeon Tiles */
#define TILE_WALL_FRONT                                                        \
  (Tile) { WALL_FRONT, 4, 214 }

#define TILE_CAVE_FLOOR_MIDDLE                                                 \
  (Tile) { CAVE_FLOOR_MIDDLE, 4, 151 }

#define TILE_DIRT_FLOOR_MIDDLE                                                 \
  (Tile) { DIRT_FLOOR_MIDDLE, 67, 214 }

// cool lookup table for all the tiles,
// we can just use the tile enum to look for the corresponding struct!
// HACK Probably not the best way to do this : (
// NOTE: adding new tiles to this array is order sensitive
static Tile tileids[NUM_TILES] = {TILE_WALL_FRONT, TILE_CAVE_FLOOR_MIDDLE,
                                  TILE_DIRT_FLOOR_MIDDLE};

#endif
