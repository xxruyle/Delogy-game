#pragma once 
#include "tile_data.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "atlas.h"


#include "dev_util.h"
#include "FastNoiseLite.h"
#include <stdlib.h>     /* srand, rand */


#define WORLD_SIZE 10
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
    
    
    Vector2 srcCoordinate; // the src coordinate in absolute world space  
    TileChunk(Vector2 src) : srcCoordinate(src) {generateNoise();}; 


    void draw(Atlas& atlas);
};



struct TileManager  
{

    std::vector<TileChunk> chunks;


    void generateChunks(); // given a spawn position of the player generate chunks around the player


    void drawAllChunks(Atlas& atlas, Vector2 playerPos); 


    // std::vector<Vector2> getNearbyChunks(Vector2 playerPos); 

    // void generateNearbyChunks(); 
};