#pragma once 
#include "tile_data.h"
#include <iostream>
#include "raylib.h"
#include "atlas.h"

#include "FastNoiseLite.h"
#include <stdlib.h>     /* srand, rand */

#define CHUNK_SIZE 30
#define DATA_STRIDE 3 

int getIndex(int i, int j);





struct TileChunk 
{
    int tiles[CHUNK_SIZE * CHUNK_SIZE * DATA_STRIDE];  // tiles will always have a size of size * stride 

    int size = 10;  
    int stride = 3; // the amount of data for each tile, before approaching the next piece of data 
    
    int row = DATA_STRIDE * CHUNK_SIZE;    // the amount of data for each row (the stride of each row)

    // given a row i and a col j, return the index of the stride in the tiles array  
    // int getIndex(int i, int j);   
    void fillTiles();
    void generateNoise();
    
    
    Vector2 srcCoordinate = {0,0}; // the src coordinate in absolute world space  
    TileChunk(Vector2 src) : srcCoordinate(src) {generateNoise();};


    void draw(Atlas& atlas);
};



struct TileManager  
{
    static const int chunksSize = 100; // the length of the chunks we will have 
    TileManager();
    // TileChunk tileChunks[chunksSize]; 


    void generateAllChunks() 
    {

    }

};