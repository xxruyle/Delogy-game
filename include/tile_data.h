#pragma once 

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
    RAIL_V, // rail vertical 
    RAIL_H, // rail horizontal 
};


/* Dungeon Tiles */ 
#define TILE_WALL_FRONT  CLITERAL(Tile){WALL_FRONT, 16, 240}    
#define TILE_WALL_N  CLITERAL(Tile){WALL_N, 16, 224}    
#define TILE_WALL_E  CLITERAL(Tile){WALL_E, 64, 240}    
#define TILE_WALL_S  CLITERAL(Tile){WALL_S, 16, 304}    
#define TILE_WALL_W  CLITERAL(Tile){WALL_W, 0, 240}    

#define TILE_WALL_NW CLITERAL(Tile){WALL_NW, 0, 224}
#define TILE_WALL_NE CLITERAL(Tile){WALL_NE, 64, 224}
#define TILE_WALL_SE CLITERAL(Tile){WALL_SE, 64, 304}
#define TILE_WALL_SW CLITERAL(Tile){WALL_SW, 0, 304}

 



#define TILE_FLOOR_MIDDLE  CLITERAL(Tile){FLOOR_MIDDLE, 32, 272}    
#define TILE_FLOOR_N  CLITERAL(Tile){FLOOR_N, 32, 256}    
#define TILE_FLOOR_E  CLITERAL(Tile){FLOOR_E, 48, 272}    
#define TILE_FLOOR_S  CLITERAL(Tile){FLOOR_S, 32, 288}    
#define TILE_FLOOR_W  CLITERAL(Tile){FLOOR_W, 16, 272}    

#define TILE_FLOOR_NW CLITERAL(Tile){FLOOR_NW, 16, 256}
#define TILE_FLOOR_NE CLITERAL(Tile){FLOOR_NE, 48, 256}
#define TILE_FLOOR_SW CLITERAL(Tile){FLOOR_SW, 16, 288}
#define TILE_FLOOR_SE CLITERAL(Tile){FLOOR_SE, 48, 288}

#define TILE_COAL1 CLITERAL(Tile){COAL1, 0, 128}

typedef struct Tile {
    int id; 
    int x; 
    int y; 
} Tile;  






