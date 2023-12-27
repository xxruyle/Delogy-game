#pragma once
#include "atlas.hpp"
#include "macros_util.hpp"
#include "player.hpp"
#include "raylib.h"

int getIndex(int x, int y);

class TileChunk {
  public:
    Vector2 srcCoordinate; // the src coordinate in chunk coordinates
    TileChunk(Vector2 src, int seed) : srcCoordinate(src) { generateNoise(seed); };

    int tileID[CHUNK_SIZE * CHUNK_SIZE]; // the id of the tile
    /* The coordinate locations of the tiles in the default atlas */
    int tileZ[CHUNK_SIZE * CHUNK_SIZE]; // the z level the current tile is on

    int itemID[CHUNK_SIZE * CHUNK_SIZE] = {0}; // the item ids
    void generateNoise(int seed);              // generate a chunk's tiles given tile manager's seed

    void drawTile(Atlas &atlas, int x, int y); // draw  an existing tile
    void drawItem(Atlas &atlas, int x, int y); // Draw an existing entity
    void draw(Atlas &atlas);                   // Draw all the tiles and entities

    void deleteTile(int x, int y); // Deletes the tile at the current z level and
    // replaces it with a new tile if applicable
    void updateTile(int x, int y);
    void updateItem(int x, int y);
};

class TileManager {
  public:
    int world_seed;
    TileManager(int random_seed) : world_seed(random_seed){};
    std::vector<TileChunk> chunks;

    int getChunkIndex(int x, int y); // Given a chunk coordinate, return the index
    // of the chunk in the chunks vector
    bool chunkExists(Vector2 chunkPos); // Check if the chunk exists in the chunks vector

    void generateChunks();
    std::vector<Vector2> generateNearbyChunks(Vector2 playerPos);

    void checkPlayerInteraction(Player &player); // change player state based on player interaction
    void drawAllChunks(Atlas &atlas, Vector2 &playerPos);
    void update(Atlas &atlas, Player &player);
};
