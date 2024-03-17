#pragma once
#include "raylib.h"
#include <string>

std::string getVector2String(Vector2 vec);

/* Drawing Dev Utility  Functions*/
void drawGameInfo(
    Camera2D &camera, Vector2 playerPos,
    Vector2
        &playerVelocity); // draws version info, fps, etc void
                          // drawMouseGridOutline(Camera2D &camera, Color
                          // color); // draw the outline of a grid depending on
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
