#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raylib.h"

void drawGameInfo()
{
    DrawText("Delogy Pre-Alpha 1.0", 2, 0, 20, RAYWHITE);
    DrawFPS(2, 25);
}

void drawMouseGridOutline(Camera2D &camera, Color color)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    // help with negative coordinate accuracy
    if (mousePos.x < 0) {
        mousePos.x = (int)floor(mousePos.x / 16) * 16;
    }
    else {
        mousePos.x = (int)(mousePos.x / 16) * 16;
    }

    if (mousePos.y < 0) {
        mousePos.y = (int)floor(mousePos.y / 16) * 16;
    }
    else {
        mousePos.y = (int)(mousePos.y / 16) * 16;
    }

    DrawRectangleLines(mousePos.x, mousePos.y, 16, 16, color);
}

void drawMouseChunkPosition(Camera2D &camera)
{
    Vector2 mouseChunkPos = getMouseChunkPosition(camera);
    std::string chunkPosStr = std::to_string((int)mouseChunkPos.x) + " " + std::to_string((int)mouseChunkPos.y);
    DrawText(chunkPosStr.c_str(), 2, 100, 20, RAYWHITE);
}

void drawMouseGridPosition(Camera2D &camera)
{
    Vector2 mouseGridPos = getMouseGridPosition(camera);

    std::string gridPosStr = std::to_string((int)mouseGridPos.x) + " " + std::to_string((int)mouseGridPos.y);

    DrawText(gridPosStr.c_str(), 2, 50, 20, RAYWHITE);
}

Vector2 getMouseGridPosition(Camera2D &camera)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    // help with negative coordinate accuracy
    if (mousePos.x < 0) {
        mousePos.x = (int)floor(mousePos.x / 16);
    }
    else {
        mousePos.x = (int)(mousePos.x / 16);
    }

    if (mousePos.y < 0) {
        mousePos.y = (int)floor(mousePos.y / 16);
    }
    else {
        mousePos.y = (int)(mousePos.y / 16);
    }

    return mousePos;
}

Vector2 getGridPosition(Vector2 screenWorldSpace)
{
    // help with negative coordinate accuracy
    if (screenWorldSpace.x < 0) {
        screenWorldSpace.x = (int)floor(screenWorldSpace.x / 16);
    }
    else {
        screenWorldSpace.x = (int)(screenWorldSpace.x / 16);
    }

    if (screenWorldSpace.y < 0) {
        screenWorldSpace.y = (int)floor(screenWorldSpace.y / 16);
    }
    else {
        screenWorldSpace.y = (int)(screenWorldSpace.y / 16);
    }

    return screenWorldSpace;
}

Vector2 getMouseChunkPosition(Camera2D &camera)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    mousePos = getMouseGridPosition(camera);
    return getChunkPosition(mousePos);
}

Vector2 getChunkPosition(Vector2 gridPosition)
{
    return Vector2{(float)floor(gridPosition.x / CHUNK_SIZE), (float)floor(gridPosition.y / CHUNK_SIZE)};
}

Vector2 getRelativeChunkGridPosition(Vector2 chunkSrc, Vector2 gridPosition)
{
    Vector2 chunkSrcGridPos = chunkToGrid(chunkSrc);
    Vector2 relativeChunkPos = {gridPosition.x - chunkSrcGridPos.x, gridPosition.y - chunkSrcGridPos.y};
    return relativeChunkPos;
}

Vector2 chunkToGrid(Vector2 chunkPosition)
{
    return Vector2{chunkPosition.x * CHUNK_SIZE, chunkPosition.y * CHUNK_SIZE};
}

Vector2 chunkToWorldSpace(Vector2 chunkPosition)
{
    return Vector2{chunkPosition.x * CHUNK_SIZE * 16, chunkPosition.y * CHUNK_SIZE * 16};
}
