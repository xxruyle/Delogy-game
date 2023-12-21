#pragma once
#include <math.h>
#include "raylib.h"
#include "tile_manager.h"
#include <string>

void drawMouseGridOutline(Camera2D& camera, Color color); // draw the outline of a grid depending on the coordinate the mouse is hovering over 
void drawMouseChunkPosition(Camera2D& camera);
void drawMouseGridPosition(Camera2D& camera, Vector2 mouseGridPos);
void drawGameInfo(); // draws version info, fps, etc 

Vector2 getMouseGridPosition(Camera2D& camera);
Vector2 getMouseChunkPosition(Camera2D& camera, int chunkSize);
Vector2 getGridPosition(Vector2 screenWorldSpace); // convert absolute coordinate space to absolute grid space coordinates 
Vector2 getChunkPosition(Vector2 gridPosition, int chunkSize); // convert from absolute grid space coordinates to chunk coordinates 
Vector2 chunkToGrid(Vector2 chunkPosition, int chunkSize); // convert from chunk coordinates to absolute grid space coordinates
Vector2 chunkToWorldSpace(Vector2 chunkPosition, int chunkSize); // convert from chunk coordinates to absolute coordinate space 
