#pragma once 
#include "macros_util.h"

#include "raylib.h"
#include "raymath.h"
#include "tile_data.h"
#include "atlas.h"
#include "dev_util.h"
#include "FastNoiseLite.h"


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
    int renderDistance = 3;// the amount of chunks near the player the tile manager will render 
    std::vector<TileChunk> chunks;


    int getChunkIndex(int x, int y); // Given a chunk coordinate, return the index of the chunk in the chunks vector
    bool chunkExists(Vector2 chunkPos); // Check if the chunk exists in the chunks vector

    void generateChunks(); // given a spawn position of the player generate chunks around the player
    void drawAllChunks(Atlas& atlas, Vector2 playerPos); 

    std::vector<Vector2> generateNearbyChunks(Vector2 playerPos); 
};