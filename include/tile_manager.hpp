#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "tile_data.hpp"
#include "ui.hpp"

int getIndex(int x, int y);

class TileChunk {
  public:
    Vector2 srcCoordinate; // the src coordinate in chunk coordinates
    TileChunk(Vector2 src, int seed) : srcCoordinate(src) {
        wallGeneration();
        drunkardWalk();
    };

    int tileID[CHUNK_SIZE * CHUNK_SIZE]; // the id of the tile
    /* The coordinate locations of the tiles in the default atlas */
    int tileZ[CHUNK_SIZE * CHUNK_SIZE]; // the z level the current tile is on

    int itemID[CHUNK_SIZE * CHUNK_SIZE] = {0}; // the item ids, they always have the highest implicit z level
    void generateNoise(int seed);              // generate a chunk's tiles given tile manager's seed
    void drunkardWalk();
    void wallGeneration();

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

    void checkPlayerInteraction(InputSystem input, Camera2D &camera, UI &ui,
                                InventoryC &playerInventory); // change player state based on player interaction

    std::vector<Vector2> generateNearbyChunks(Vector2 playerPos);
    void drawAllChunks(Atlas &atlas, Vector2 &playerPos);
    int getItemUnder(Vector2 pos);
    void update(Atlas &atlas, InputSystem input, UI &ui, Scene &scene);
};
