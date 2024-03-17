#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include "scene.hpp"
#include "ui.hpp"
#include <iostream>

struct IndexPair {
  int chunk; // the index of the chunk contained in tilemanager
  int tile;  // the index of the tile contained in tileIDs in tilechunk
};

int getIndex(int x, int y);
Vector2 getRandomDirection();

class TileChunk {
public:
  Vector2 srcCoordinate; // the src coordinate in chunk coordinates
  TileChunk(Vector2 src, int seed) : srcCoordinate(src) { wallGeneration(); };

  int tileID[CHUNK_SIZE * CHUNK_SIZE]; // the id of the tile
  /* The coordinate locations of the tiles in the default atlas */
  int tileZ[CHUNK_SIZE * CHUNK_SIZE]; // the z level the current tile is on

  int itemID[CHUNK_SIZE * CHUNK_SIZE] = {0}; // the item ids, they always have
                                             // the highest implicit z level
  void wallGeneration();

  void drawTile(Atlas &atlas, int x, int y); // draw  an existing tile
  void drawItem(Atlas &atlas, int x, int y); // Draw an existing entity
  void draw(Atlas &atlas);                   // Draw all the tiles and entities

  void deleteAtTile(int x, int y); // Deletes the tile or item (if it exists) at
                                   // the current z level and
  void updateTile(int x, int y);
  void updateItem(int x, int y, int playerItemID);
};

class TileManager {
public:
  int world_seed;
  int renderDistance = RENDER_DISTANCE;
  TileManager(int random_seed) : world_seed(random_seed) {
    std::cout << "World seed: " << random_seed << std::endl;
  };
  std::vector<TileChunk> chunks;

  int getChunkIndex(int x, int y); // Given a chunk coordinate, return the index
  // of the chunk in the chunks vector
  bool chunkExists(
      Vector2 chunkPos); // Check if the chunk exists in the chunks vector

  void generateChunks();

  void drunkardGenerateAll();

  void checkPlayerInteraction(
      Camera2D &camera, UI &ui,
      InventoryC
          &playerInventory); // change player state based on player interaction

  void checkDevInput();

  std::vector<Vector2> getNearbyChunks(Vector2 playerPos, int distance);
  void drawAllChunks(Atlas &atlas, Vector2 &playerPos);
  int getItemUnder(Vector2 pos);
  std::vector<Vector2> getNeighbors(
      int x, int y,
      int radius); // find neighbors around an absolute grid space coordinate
  bool isValidCoordinate(int x, int y);
  void generateOres();

  IndexPair getIndexPair(int x,
                         int y); // returns chunk index and relative grid pos
                                 // index  given absolute grid space coordinate

  IndexPair getGridIndexPair(int x, int y);

  void update(Atlas &atlas, UI &ui, Scene &scene);
};
