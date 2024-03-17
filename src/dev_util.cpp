#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <math.h>
#include <string>

std::string getVector2String(Vector2 vec)
{
    return "(" + std::to_string((int)vec.x) + ", " + std::to_string((int)vec.y) + ")";
}

void drawGameInfo(Camera2D &camera, Vector2 playerPos, Vector2 &playerVelocity)
{
    DrawText("Delogy Indev 1.0", 3, 0, UI_FONT_SIZE, RAYWHITE);
    DrawFPS(2, 25);
    drawMouseChunkPosition(camera);
    drawMouseGridPosition(camera);
    drawPlayerGridPosition(playerPos);

    std::string playerVelocityStr =
        "Velocity: (" + std::to_string((int)playerVelocity.x) + ", " + std::to_string((int)playerVelocity.y) + ")";
    DrawText(playerVelocityStr.c_str(), 3, 125, UI_FONT_SIZE, RAYWHITE);
}

void drawCounter(const char *text, int count)
{
    std::string strCount = std::to_string(count);
    std::string final = text + strCount;
    DrawText(final.c_str(), 3, 145, UI_FONT_SIZE, RAYWHITE);
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

    DrawRectangleLinesEx(Rectangle{mousePos.x, mousePos.y, 16, 16}, 0.4f, color);
}

void drawMouseChunkPosition(Camera2D &camera)
{
    Vector2 mouseChunkPos = getMouseChunkPosition(camera);
    std::string chunkPosStr =
        "ChunkPos: " + std::to_string((int)mouseChunkPos.x) + " " + std::to_string((int)mouseChunkPos.y);
    DrawText(chunkPosStr.c_str(), 3, 75, UI_FONT_SIZE, RAYWHITE);
}

void drawMouseGridPosition(Camera2D &camera)
{
    Vector2 mouseGridPos = getMouseGridPosition(camera);

    std::string gridPosStr = "Pos: " + std::to_string((int)mouseGridPos.x) + " " + std::to_string((int)mouseGridPos.y);

    DrawText(gridPosStr.c_str(), 3, 50, UI_FONT_SIZE, RAYWHITE);
}
void drawPlayerGridPosition(Vector2 playerPos)
{
    playerPos = getGridPosition(playerPos);
    std::string playerPosStr =
        "Player: (" + std::to_string((int)playerPos.x) + ", " + std::to_string((int)playerPos.y) + ")";

    DrawText(playerPosStr.c_str(), 3, 100, UI_FONT_SIZE, WHITE);
}

void drawChunkInfo(Vector2 chunkWorldPostion)
{
    // converting back to chunk src
    Vector2 chunkPosition = getChunkPosition(chunkWorldPostion);
    std::string pos =
        "(" + std::to_string((int)chunkPosition.x / 16) + ", " + std::to_string((int)chunkPosition.y / 16) + *")";

    DrawText(pos.c_str(), chunkWorldPostion.x, chunkWorldPostion.y, 40, RED);
    DrawRectangleLines(chunkWorldPostion.x, chunkWorldPostion.y, CHUNK_SIZE * 16, CHUNK_SIZE * 16, WHITE);
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

Vector2 getChunkPosition(Vector2 gridPosition) // given a grid position find the chunk position
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
