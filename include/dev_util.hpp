#pragma once
#include "raylib.h"
#include "ui.hpp"

/* Drawing Dev Utility  Functions*/
void drawGameInfo(
    Player &player); // draws version info, fps, etc void drawMouseGridOutline(Camera2D &camera, Color color); // draw
                     // the outline of a grid depending on // the coordinate the mouse is hovering over
void drawMouseChunkPosition(Camera2D &camera);
void drawMouseGridPosition(Camera2D &camera);
void drawPlayerGridPosition(Vector2 playerPos);
void drawMouseGridOutline(Camera2D &camera, Color color);

void drawItem(Camera2D &camera, Atlas &atlas, int itemID);
void drawChunkInfo(Vector2 chunkWorldPostion); // draws chunk outlines and chunk coordinates

/* Getting Grid Info*/
Vector2 getMouseGridPosition(Camera2D &camera);
Vector2 getGridPosition(Vector2 screenWorldSpace); // convert absolute coordinate space
// to absolute grid space coordinates

/* Getting Chunk Info */
Vector2 getMouseChunkPosition(Camera2D &camera);
Vector2 getChunkPosition(Vector2 gridPosition); // convert from absolute grid space
Vector2 getRelativeChunkGridPosition(Vector2 chunkSrc,
                                     Vector2 gridPosition); // converts absolute grid coordinate to coordinates in chunk
// coordinates to chunk coordinates
Vector2 chunkToGrid(Vector2 chunkPosition);       // convert from chunk coordinates to absolute grid space coordinates
Vector2 chunkToWorldSpace(Vector2 chunkPosition); // convert from chunk coordinates to
                                                  // absolute coordinate space
