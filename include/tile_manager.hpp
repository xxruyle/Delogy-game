#pragma once
#include "atlas.hpp"
#include "dev_util.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"

int getIndex(int x, int y);

class TileChunk {
  public:
    Vector2 srcCoordinate; // the src coordinate in chunk coordinates
    TileChunk(Vector2 src, int seed) : srcCoordinate(src) { generateNoise(seed); };

    int tileID[CHUNK_SIZE * CHUNK_SIZE]; // the id of the tile
    /* The coordinate locations of the tiles in the default atlas */
    int tileZ[CHUNK_SIZE * CHUNK_SIZE]; // the z level the current tile is on

    int itemID[CHUNK_SIZE * CHUNK_SIZE] = {0}; // the item ids, they always have the highest implicit z level
    void generateNoise(int seed);              // generate a chunk's tiles given tile manager's seed

    void drawTile(Atlas &atlas, int x, int y); // draw  an existing tile
    void drawItem(Atlas &atlas, int x, int y); // Draw an existing entity
    void draw(Atlas &atlas);                   // Draw all the tiles and entities

    void deleteAtTile(int x, int y); // Deletes the tile or item (if it exists) at the current z level and
    void updateTile(int x, int y);
    void updateItem(int x, int y, int playerItemID);
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

    void checkPlayerInteraction(Player &player, UI &ui); // change player state based on player interaction

    std::vector<Vector2> generateNearbyChunks(Vector2 playerPos);
    void drawAllChunks(Atlas &atlas, Vector2 &playerPos);
    int getItemUnder(Vector2 pos);
    void update(Atlas &atlas, Player &player, UI &ui);
};
