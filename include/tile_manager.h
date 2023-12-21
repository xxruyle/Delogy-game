#pragma once 
#include "macros_util.h"

#include "raylib.h"
#include "raymath.h"
#include "tile_data.h"
#include "atlas.h"
#include "dev_util.h"
#include "FastNoiseLite.h"

int getIndex(int x, int y); // given an x and y coordinate in a chunk return the index of the tile

struct TileChunk 
{
    Vector2 srcCoordinate; // the src coordinate in chunk coordinates  
    TileChunk(Vector2 src, int seed) : srcCoordinate(src) {generateNoise(seed);}; 

    int tileID[CHUNK_SIZE * CHUNK_SIZE]; // the id of the tile 
    /* l The coordinate locations of the tiles in the default atlas */
    int tileX[CHUNK_SIZE * CHUNK_SIZE]; 
    int tileY[CHUNK_SIZE * CHUNK_SIZE]; 

    int entities[CHUNK_SIZE * CHUNK_SIZE];
    
    void generateNoise(int seed); // generate a chunk's tiles given tile manager's seed

    void draw(Atlas& atlas); // Draw all the tiles
    void drawEntities(Atlas& atlas); // Draw all the existing entities 
};



struct TileManager  
{
    int world_seed;
    TileManager(int random_seed) : world_seed(random_seed) {};
    std::vector<TileChunk> chunks;

    int getChunkIndex(int x, int y); // Given a chunk coordinate, return the index of the chunk in the chunks vector
    bool chunkExists(Vector2 chunkPos); // Check if the chunk exists in the chunks vector

    void generateChunks(); // given a spawn position of the player generate chunks around the player
    void drawAllChunks(Atlas& atlas, Vector2 playerPos); 

    std::vector<Vector2> generateNearbyChunks(Vector2 playerPos); 
};