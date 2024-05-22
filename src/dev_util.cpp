#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include <math.h>
#include <string>
#include <regex>

std::string getVector2String(Vector2 vec)
{
    float x = vec.x;
    float y = vec.y;

    std::string xstr = std::to_string(x);
    std::string ystr = std::to_string(y);
    xstr.erase(xstr.find('.') + 1); //chop to precision
    xstr.erase(xstr.find_last_not_of('.') + 1, std::string::npos);

    ystr.erase(ystr.find('.') + 1); //chop to precision
    ystr.erase(ystr.find_last_not_of('.') + 1, std::string::npos);

    std::string vecString = "(" + xstr + ", " + ystr + ")";
    return vecString;
}
int Vector2Manhattan(Vector2 vec1, Vector2 vec2) { return abs(vec1.x - vec2.x) + abs(vec1.y - vec2.y); }
std::vector<Vector2> getNearNeighbors(Vector2 curPos)
{ // get the 8 neighbors of a cell
    std::vector<Vector2> neighbors;
    neighbors.push_back({curPos.x, curPos.y - 1}); // North
    neighbors.push_back({curPos.x + 1, curPos.y}); // East
    neighbors.push_back({curPos.x, curPos.y + 1}); // South
    neighbors.push_back({curPos.x - 1, curPos.y}); // West
    /* neighbors.push_back({curPos.x - 1, curPos.y - 1}); // Northwest */
    /* neighbors.push_back({curPos.x + 1, curPos.y - 1}); // Northeast */
    /* neighbors.push_back({curPos.x + 1, curPos.y + 1}); // Southeast */
    /* neighbors.push_back({curPos.x - 1, curPos.y + 1}); // Southwest */
    return neighbors;
}

std::vector<Vector2> getRadiusNeighbors(Vector2 curPos, int radius)
{
    std::vector<Vector2> neighbors;

    for (int row = curPos.y - radius; row < curPos.y + radius; row++) {
        for (int col = curPos.x - radius; col < curPos.x + radius; col++) {
            neighbors.push_back({col, row});
        }
    }

    return neighbors;
}

// get neighbors of curpos in a spiral order (farther distance will be pushed later)
std::vector<Vector2> getSpiralNeighbors(Vector2 curPos, int radius)
{
    std::vector<Vector2> neighbors;

    // Start from the center
    neighbors.push_back(curPos);

    // Directions for moving in the order: right, up, left, down
    int directionX[4] = {1, 0, -1, 0};
    int directionY[4] = {0, 1, 0, -1};

    int x = 0, y = 0; // Start at the origin relative to curPos
    int stepSize = 1; // Initial step size

    while (stepSize <= radius) {
        for (int direction = 0; direction < 4; ++direction) {
            for (int step = 0; step < stepSize; ++step) {
                x += directionX[direction];
                y += directionY[direction];

                // Add to neighbors if within radius
                if (std::abs(x) <= radius && std::abs(y) <= radius) {
                    neighbors.push_back({curPos.x + x, curPos.y + y});
                }
            }
            // Increase step size after completing a full loop in all four directions
            if (direction == 1 || direction == 3) {
                ++stepSize;
            }
        }
    }

    return neighbors;
}

void drawGameInfo(Camera2D& camera, Vector2 playerPos, Vector2& playerVelocity)
{
    int UI_FONT_SIZE = LuaGetInt("UI_FONT_SIZE", "scripts/game_settings.lua");
    DrawText("Delogy Indev 1.0", 3, 0, UI_FONT_SIZE, RAYWHITE);
    DrawFPS(2, 25);
    drawMouseChunkPosition(camera);
    drawMouseGridPosition(camera);
    drawPlayerGridPosition(playerPos);

    std::string playerVelocityStr = "Velocity: (" + std::to_string((int)playerVelocity.x) + ", " + std::to_string((int)playerVelocity.y) + ")";
    DrawText(playerVelocityStr.c_str(), 3, 125, UI_FONT_SIZE, RAYWHITE);
}

void drawCounter(const char* text, int count)
{
    int UI_FONT_SIZE = LuaGetInt("UI_FONT_SIZE", "scripts/game_settings.lua");
    std::string strCount = std::to_string(count);
    std::string final = text + strCount;
    DrawText(final.c_str(), 3, 145, UI_FONT_SIZE, RAYWHITE);
}

void drawMouseGridOutline(Camera2D& camera, Color color)
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

    DrawRectangleLinesEx(Rectangle{mousePos.x, mousePos.y, 16, 16}, 0.2f, color);
}

void drawMouseChunkPosition(Camera2D& camera)
{
    int UI_FONT_SIZE = LuaGetInt("UI_FONT_SIZE", "scripts/game_settings.lua");
    Vector2 mouseChunkPos = getMouseChunkPosition(camera);
    std::string chunkPosStr = "ChunkPos: " + std::to_string((int)mouseChunkPos.x) + " " + std::to_string((int)mouseChunkPos.y);
    DrawText(chunkPosStr.c_str(), 3, 75, UI_FONT_SIZE, RAYWHITE);
}

void drawMouseGridPosition(Camera2D& camera)
{
    Vector2 mouseGridPos = getMouseGridPosition(camera);

    std::string gridPosStr = "Pos: " + std::to_string((int)mouseGridPos.x) + " " + std::to_string((int)mouseGridPos.y);

    int UI_FONT_SIZE = LuaGetInt("UI_FONT_SIZE", "scripts/game_settings.lua");
    DrawText(gridPosStr.c_str(), 3, 50, UI_FONT_SIZE, RAYWHITE);
}
void drawPlayerGridPosition(Vector2 playerPos)
{
    playerPos = getGridPosition(playerPos);
    std::string playerPosStr = "Player: (" + std::to_string((int)playerPos.x) + ", " + std::to_string((int)playerPos.y) + ")";

    int UI_FONT_SIZE = LuaGetInt("UI_FONT_SIZE", "scripts/game_settings.lua");
    DrawText(playerPosStr.c_str(), 3, 100, UI_FONT_SIZE, WHITE);
}

void drawChunkInfo(Vector2 chunkWorldPostion)
{
    // converting back to chunk src
    Vector2 chunkPosition = getChunkPosition(chunkWorldPostion);
    std::string pos = "(" + std::to_string((int)chunkPosition.x / 16) + ", " + std::to_string((int)chunkPosition.y / 16) + *")";

    DrawText(pos.c_str(), chunkWorldPostion.x, chunkWorldPostion.y, 40, RED);
    DrawRectangleLines(chunkWorldPostion.x, chunkWorldPostion.y, CHUNK_SIZE * 16, CHUNK_SIZE * 16, WHITE);
}

Vector2 getMouseGridPosition(Camera2D& camera)
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

Vector2 getMinimapGridPos(Camera2D& camera, Vector2 screenMousePos, float tileSize)
{
    Vector2 mousePos = GetScreenToWorld2D(screenMousePos, camera);

    // help with negative coordinate accuracy
    if (mousePos.x < 0) {
        mousePos.x = (int)floor(mousePos.x / tileSize);
    }
    else {
        mousePos.x = (int)(mousePos.x / tileSize);
    }

    if (mousePos.y < 0) {
        mousePos.y = (int)floor(mousePos.y / tileSize);
    }
    else {
        mousePos.y = (int)(mousePos.y / tileSize);
    }

    return mousePos;
}

float CosineInterpolate(float y1, float y2, float mu)
{
    float mu2;

    mu2 = (1 - cos(mu * PI)) / 2;
    return (y1 * (1 - mu2) + y2 * mu2);
}

Vector2 getMouseChunkPosition(Camera2D& camera)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    mousePos = getMouseGridPosition(camera);
    return getChunkPosition(mousePos);
}

Vector2 getGridToScreenPos(Vector2 gridPos) { return {gridPos.x * 16, gridPos.y * 16}; }

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

Vector2 chunkToGrid(Vector2 chunkPosition) { return Vector2{chunkPosition.x * CHUNK_SIZE, chunkPosition.y * CHUNK_SIZE}; }

Vector2 chunkToWorldSpace(Vector2 chunkPosition) { return Vector2{chunkPosition.x * CHUNK_SIZE * 16, chunkPosition.y * CHUNK_SIZE * 16}; }
