#pragma once
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <cmath>
#include <limits>
#include <string>
#include <vector>

struct Vector2Util {

  size_t operator()(const Vector2 &vec) const {
    size_t h1 = std::hash<float>{}(vec.x);
    size_t h2 = std::hash<float>{}(vec.y);

    return h1 ^ (h2 << 1);
  }

  bool operator()(const Vector2 &vec1, const Vector2 &vec2) const {
    return vec1.x == vec2.x && vec1.y == vec2.y;
  }
};

struct PathNode {
  Vector2 pos;
  float cost = std::numeric_limits<float>::max();
};

struct PathNodeComparison {
  bool operator()(PathNode &pNode1, PathNode &pNode2) {
    return pNode1.cost > pNode2.cost;
  }
};

std::string getVector2String(Vector2 vec);
int Vector2Manhattan(Vector2 vec1, Vector2 vec2);

std::vector<Vector2>
getNearNeighbors(Vector2 curPos); // get the 8 neighbors of a cell

std::vector<Vector2> getRadiusNeighbors(Vector2 curPos, int radius);

/* Drawing Dev Utility  Functions*/
void drawGameInfo(
    Camera2D &camera, Vector2 playerPos,
    Vector2 &playerVelocity); // draws version info, fps, etc void
                              // drawMouseGridOutline(Camera2D &camera, Color
                              // color); // draw the outline of a grid depending
                              // on
                              // // the coordinate the mouse is hovering over

void drawCounter(const char *text, int count);
void drawMouseChunkPosition(Camera2D &camera);
void drawMouseGridPosition(Camera2D &camera);
void drawPlayerGridPosition(Vector2 playerPos);

void drawMouseGridOutline(Camera2D &camera, Color color);
void drawChunkInfo(
    Vector2 chunkWorldPostion); // draws chunk outlines and chunk coordinates

/*
World Space: Where it is in world coordinates (Ex: a player at the cell (1,2)
would be at (1 * 16, 2 * 16) in absolute world coordinates)
- used for precise things like collision

Absolute Grid Space: Simply the which absolute cell the player is in (Ex: player
is in cell (0, 0))

Relative Chunk Grid Position: which tile/cell the player is at relative to the
chunk the player is in
- Ex: player could be at 33, 33  in absolute grid space but relative to the
player's chunk that could be (0, 0)
*/

/* Getting Grid and world Info*/
Vector2 getMouseGridPosition(Camera2D &camera);
Vector2
getGridPosition(Vector2 screenWorldSpace); // convert absolute coordinate space
// to absolute grid space coordinates

Vector2 getMinimapGridPos(Camera2D &camera, Vector2 screenMousePos,
                          float tileSize);

Vector2 getGridToScreenPos(Vector2 gridPos);

/* Getting Chunk Info */
Vector2 getMouseChunkPosition(Camera2D &camera);
Vector2
getChunkPosition(Vector2 gridPosition); // convert from absolute grid space
Vector2 getRelativeChunkGridPosition(
    Vector2 chunkSrc,
    Vector2 gridPosition); // converts absolute grid coordinate to coordinates
                           // in chunk
// coordinates to chunk coordinates
Vector2 chunkToGrid(Vector2 chunkPosition); // convert from chunk coordinates to
                                            // absolute grid space coordinates
Vector2
chunkToWorldSpace(Vector2 chunkPosition); // convert from chunk coordinates to
                                          // absolute coordinate space
