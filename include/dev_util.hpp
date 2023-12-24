#pragma once
#include "raylib.h"

/* Drawing Dev Utility  Functions*/
void drawMouseGridOutline(Camera2D &camera, Color color); // draw the outline of a grid depending on
// the coordinate the mouse is hovering over
void drawMouseChunkPosition(Camera2D &camera);
void drawMouseGridPosition(Camera2D &camera);
void drawGameInfo(); // draws version info, fps, etc

/* Getting grid, chunk and coordinate info */
Vector2 getMouseGridPosition(Camera2D &camera);
Vector2 getMouseChunkPosition(Camera2D &camera);
Vector2 getGridPosition(Vector2 screenWorldSpace); // convert absolute coordinate space
// to absolute grid space coordinates
Vector2 getChunkPosition(Vector2 gridPosition); // convert from absolute grid space
Vector2 getRelativeChunkGridPosition(Vector2 chunkSrc,
                                     Vector2 gridPosition); // converts absolute grid coordinate to coordinates in chunk
// coordinates to chunk coordinates
Vector2 chunkToGrid(Vector2 chunkPosition);       // convert from chunk coordinates to absolute grid space coordinates
Vector2 chunkToWorldSpace(Vector2 chunkPosition); // convert from chunk coordinates to
// absolute coordinate space
