#pragma once 
#include "tile_data.h"
#include <iostream>
#include "raylib.h"
#include "atlas.h"


struct TileChunk 
{
    int tiles[300];  // tiles will always have a size of size * stride 

    int size = 10;  
    int stride = 3; // the amount of data for each tile, before approaching the next piece of data 
    
    int row = stride * size;    

    // given a row i and a col j, return the index of the stride in the tiles array  
    int getIndex(int i, int j);  
    void fillTiles();
    void draw(Atlas& atlas);
};



struct TileManager  
{
    static const int chunksSize = 30; // the amount of chunks we will have   
    TileManager();
    TileChunk tileChunks[chunksSize];

    void drawChunks();
};