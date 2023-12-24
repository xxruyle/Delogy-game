#ifndef TILE_DATA_H
#define TILE_DATA_H
enum TileType {
    WALL_FRONT,

    WALL_N,
    WALL_E,
    WALL_S,
    WALL_W,

    WALL_NW,
    WALL_NE,
    WALL_SE,
    WALL_SW,

    FLOOR_MIDDLE,

    FLOOR_N,
    FLOOR_E,
    FLOOR_S,
    FLOOR_W,

    FLOOR_NW,
    FLOOR_NE,
    FLOOR_SE,
    FLOOR_SW,

    COAL1,
    COAL2,
    RUBY1,
    RUBY2,

    RAIL_NW, // top left rail
    RAIL_NE, // top right rail
    RAIL_SW, // bottom left rail
    RAIL_SE, // bottom right rail
    RAIL_V,  // rail vertical
    RAIL_H,  // rail horizontal
};

#define NUM_TILES 30 // the number of tiles so far

/* Dungeon Tiles */
#define TILE_WALL_FRONT                                                                                                \
    CLITERAL(Tile) { WALL_FRONT, 16, 240 }
#define TILE_WALL_N                                                                                                    \
    CLITERAL(Tile) { WALL_N, 16, 224 }
#define TILE_WALL_E                                                                                                    \
    CLITERAL(Tile) { WALL_E, 64, 240 }
#define TILE_WALL_S                                                                                                    \
    CLITERAL(Tile) { WALL_S, 16, 304 }
#define TILE_WALL_W                                                                                                    \
    CLITERAL(Tile) { WALL_W, 0, 240 }

#define TILE_WALL_NW                                                                                                   \
    CLITERAL(Tile) { WALL_NW, 0, 224 }
#define TILE_WALL_NE                                                                                                   \
    CLITERAL(Tile) { WALL_NE, 64, 224 }
#define TILE_WALL_SE                                                                                                   \
    CLITERAL(Tile) { WALL_SE, 64, 304 }
#define TILE_WALL_SW                                                                                                   \
    CLITERAL(Tile) { WALL_SW, 0, 304 }

#define TILE_FLOOR_MIDDLE                                                                                              \
    CLITERAL(Tile) { FLOOR_MIDDLE, 32, 272 }
#define TILE_FLOOR_N                                                                                                   \
    CLITERAL(Tile) { FLOOR_N, 32, 256 }
#define TILE_FLOOR_E                                                                                                   \
    CLITERAL(Tile) { FLOOR_E, 48, 272 }
#define TILE_FLOOR_S                                                                                                   \
    CLITERAL(Tile) { FLOOR_S, 32, 288 }
#define TILE_FLOOR_W                                                                                                   \
    CLITERAL(Tile) { FLOOR_W, 16, 272 }

#define TILE_FLOOR_NW                                                                                                  \
    CLITERAL(Tile) { FLOOR_NW, 16, 256 }
#define TILE_FLOOR_NE                                                                                                  \
    CLITERAL(Tile) { FLOOR_NE, 48, 256 }
#define TILE_FLOOR_SE                                                                                                  \
    CLITERAL(Tile) { FLOOR_SE, 48, 288 }
#define TILE_FLOOR_SW                                                                                                  \
    CLITERAL(Tile) { FLOOR_SW, 16, 288 }

#define TILE_COAL1                                                                                                     \
    CLITERAL(Tile) { COAL1, 0, 128 }

#define TILE_RAIL_V                                                                                                    \
    CLITERAL(Tile) { RAIL_V, 0, 176 }

struct Tile {
    int id;
    /* default_atlas.png positions */
    int x;
    int y;
};

// cool lookup table for all the tiles,
// we can just use the tile enum to look for the corresponding struct!
// HACK Probably not the best way to do this : (
// NOTE: adding new tiles to this array is order sensitive
static Tile tileids[NUM_TILES] = {TILE_WALL_FRONT,

                                  TILE_WALL_N,       TILE_WALL_E,   TILE_WALL_S,   TILE_WALL_W,

                                  TILE_WALL_NW,      TILE_WALL_NE,  TILE_WALL_SE,  TILE_WALL_SW,

                                  TILE_FLOOR_MIDDLE,

                                  TILE_FLOOR_N,      TILE_FLOOR_E,  TILE_FLOOR_S,  TILE_FLOOR_W,

                                  TILE_FLOOR_NW,     TILE_FLOOR_NE, TILE_FLOOR_SE, TILE_FLOOR_SW,

                                  TILE_COAL1,

                                  TILE_RAIL_V};
#endif
