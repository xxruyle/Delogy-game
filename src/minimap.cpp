#include "minimap.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_manager.hpp"
#include "ui_util.hpp"
#include "macros_util.hpp"
#include "input_system.hpp"
#include <vector>
#include "dev_util.hpp"

bool MiniMap::inMinimap(Vector2 ePosition, Vector2 mapOrigin)
{
    mapOrigin.x *= 16;
    mapOrigin.y *= 16;

    int gridMapWidth = mapRadius * 16 * 2;

    return ((ePosition.x >= mapOrigin.x && ePosition.x <= mapOrigin.x + gridMapWidth - 50) &&
            (ePosition.y >= mapOrigin.y && ePosition.y <= mapOrigin.y + gridMapWidth - 50));
}

void MiniMap::drawFullscreenMinimap(Vector2 playerPos)
{
    float cellSize = position.width / (mapRadius * 2);
    for (MapCell cell : cellInfo) {
        int id = cell.id;
        int itemID = cell.item;
        int xPos = cell.screenX;
        int yPos = cell.screenY;

        if (id == 0) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{46, 34, 47, 200});
        }
        else if (id == 2) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{72, 48, 40, 200});
        }

        if (itemID >= 1 && itemID <= 6) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{74, 79, 86, 255});
        }
    }

    if (inMinimap(playerPos, cellInfo[0].gridPos)) {
        Vector2 playerGrid = {playerPos.x / 16, playerPos.y / 16};
        Vector2 projectedPos = {position.x + (playerGrid.x - cellInfo[0].gridPos.x) * cellSize,
                                position.y + (playerGrid.y - cellInfo[0].gridPos.y) * cellSize};

        DrawRectangle(projectedPos.x, projectedPos.y, 20, 20, RED);
    }
    else {
        updateMap = true;
    }
}

void MiniMap::drawMinimap(Vector2 playerPos)
{
    float cellSize = position.width / (mapRadius * 2);
    for (MapCell cell : cellInfo) {
        int id = cell.id;
        int itemID = cell.item;
        int xPos = cell.screenX;
        int yPos = cell.screenY;

        if (id == 0) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{46, 34, 47, 255});
        }
        else if (id == 2) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{72, 48, 40, 255});
        }

        if (itemID >= 1 && itemID <= 6) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{74, 79, 86, 255});
        }
    }

    if (inMinimap(playerPos, cellInfo[0].gridPos)) {
        Vector2 playerGrid = {playerPos.x / 16, playerPos.y / 16};
        Vector2 projectedPos = {position.x + (playerGrid.x - cellInfo[0].gridPos.x) * cellSize,
                                position.y + (playerGrid.y - cellInfo[0].gridPos.y) * cellSize};

        DrawRectangle(projectedPos.x, projectedPos.y, 10, 10, RED);
    }
    else {
        updateMap = true;
    }
}

void MiniMap::populateMap(TileManager &tileManager, Vector2 playerPos)
{
    cellInfo.clear();

    Vector2 playerGrid = getGridPosition(playerPos);

    std::vector<Vector2> gridPositions = tileManager.getNeighbors(playerGrid.x, playerGrid.y, mapRadius);

    int xMapPos = 0;
    int yMapPos = 0;
    int prevY = gridPositions[0].y * 16;
    for (Vector2 gridPos : gridPositions) {
        IndexPair indexPair = tileManager.getIndexPair(gridPos.x * 16, gridPos.y * 16);
        int id = tileManager.chunks[indexPair.chunk].tileID[indexPair.tile];
        int item = tileManager.chunks[indexPair.chunk].itemID[indexPair.tile];
        cellInfo.push_back(MapCell{id, item, xMapPos, yMapPos, gridPos});
        int curY = gridPos.y * 16;

        if (curY != prevY) {
            xMapPos = 0;
            yMapPos++;
            prevY = curY;
        }
        else {
            xMapPos++;
        }
    }
}

void MiniMap::draw(TileManager &tileManager, Vector2 playerPos)
{
    float padding = 10.0f;

    populateMap(tileManager, playerPos);
    if (updateMap) {
        updateMap = false;
    }

    if (InputSystem::getUserKeypress() == OPEN_MINIMAP) {
        if (fullScreen) {
            fullScreen = false;
        }
        else {
            fullScreen = true;
        }
        /* updateMap = true; */
    }

    if (InputSystem::getUserMouseInteraction() == PLAYER_CREATE ||
        InputSystem::getUserMouseInteraction() == PLAYER_DESTROY) {
        updateMap = true;
    }

    if (!fullScreen) {
        position = Rectangle{GetScreenWidth() - width - padding, padding, width, height};
        UIWindowOutline(position);
        drawMinimap(playerPos);
    }
    else {
        float size = GetScreenHeight() - padding * 2;

        position = Rectangle{GetScreenWidth() - size - padding, 0 + padding, size, size};
        UIWindowOutline(position);
        drawFullscreenMinimap(playerPos);
    }
}

void MiniMap::update() {}
