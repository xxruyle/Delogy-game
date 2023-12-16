#pragma once 

enum TileType {
    FLOOR, 
    COAL1, 
    COAL2, 
    RUBY1,
    RUBY2, 
    RAIL_NW, // top left rail
    RAIL_NE, // top right rail 
    RAIL_SW, // bottom left rail 
    RAIL_SE, // bottom right rail 
    RAIL_V, // rail vertical 
    RAIL_H, // rail horizontal 
};

#define TILE_FLOOR  CLITERAL(Tile){FLOOR, 32, 128}   
#define TILE_COAL1 CLITERAL(Tile){COAL1, 0, 128}

typedef struct Tile {
    int id; 
    int x; 
    int y; 
} Tile;






