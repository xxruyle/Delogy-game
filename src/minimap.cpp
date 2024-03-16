#include "minimap.hpp"
#include "item_data.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_data.hpp"
#include "tile_manager.hpp"
#include "macros_util.hpp"
#include "input_system.hpp"
#include <vector>
#include "dev_util.hpp"

void MiniMap::drawMinimap(Vector2 playerPos, Texture2D &medium)
{

    unsigned char alphaValue = 255;
    if (fullScreen) {
        alphaValue = 220;
    }

    float cellSize = position.width / (mapRadius * 2);
    for (MapCell cell : cellInfo) {
        int id = cell.id;
        int itemID = cell.item;
        int xPos = cell.screenX;
        int yPos = cell.screenY;

        if (id == WALL_FRONT) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{46, 34, 47, alphaValue});
        }
        else if (id == CAVE_FLOOR_MIDDLE) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{132, 145, 151, alphaValue});
        }
        else if (id == DIRT_FLOOR_MIDDLE) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{72, 48, 40, alphaValue});
        }

        if (itemID >= RAIL_NW && itemID <= RAIL_SW) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{74, 79, 86, alphaValue});
        }

        if (itemID == STORAGE_BOX) {
            DrawRectangleRec({(xPos * cellSize) + position.x, (yPos * cellSize) + position.y, cellSize, cellSize},
                             Color{171, 148, 122, alphaValue});
        }
    }

    Vector2 playerGrid = {playerPos.x / 16, playerPos.y / 16};
    Vector2 projectedPos = {position.x + (playerGrid.x - cellInfo[0].gridPos.x) * cellSize,
                            position.y + (playerGrid.y - cellInfo[0].gridPos.y) * cellSize};

    /* DrawTextureRec(medium, Rectangle{12, 167, 16.0f, 14.0f}, Vector2{projectedPos.x, projectedPos.y}, WHITE); */
    DrawTexturePro(medium, Rectangle{12, 167, 16.0f, 14.0f}, Rectangle{projectedPos.x - 8, projectedPos.y - 4, 16, 14},
                   {0, 0}, 0.0f, WHITE);
    camera.target = Vector2{projectedPos.x - 4, projectedPos.y - 2};
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

void MiniMap::check_map_update(Vector2 playerPos)
{
    Vector2 cells_origin = cellInfo[0].gridPos;
    Vector2 player_grid_pos = getGridPosition(playerPos);

    float top_edge = Vector2Distance(player_grid_pos, Vector2{player_grid_pos.x, cells_origin.y});
    float right_edge = Vector2Distance(player_grid_pos, Vector2{cells_origin.x + mapRadius * 2, player_grid_pos.y});
    float bot_edge = Vector2Distance(player_grid_pos, Vector2{player_grid_pos.x, cells_origin.y + mapRadius * 2});
    float left_edge = Vector2Distance(player_grid_pos, Vector2{cells_origin.x, player_grid_pos.y});

    if (top_edge < 45 || right_edge < 45 || bot_edge < 45 || left_edge < 45) {
        updateMap = true;
    }
}

void MiniMap::draw(TileManager &tileManager, UI &ui, Vector2 playerPos, Texture2D &medium)
{
    if (updateMap) {
        populateMap(tileManager, playerPos);
        updateMap = false;
    }

    if (InputSystem::getUserMouseInteraction() == PLAYER_CREATE ||
        InputSystem::getUserMouseInteraction() == PLAYER_DESTROY) {
        updateMap = true;
    }

    if (InputSystem::getUserKeypress() == OPEN_MINIMAP) {
        if (fullScreen) { // switch to minimap
            UnloadRenderTexture(map);
            fullScreen = false;
            position = Rectangle{GetScreenWidth() - width - padding, padding, (float)width, (float)height};
            map = LoadRenderTexture(position.width, position.height);
        }
        else { // switch to bigger minimap
            UnloadRenderTexture(map);
            fullScreen = true;
            float size = GetScreenHeight() - padding * 2;
            position = Rectangle{GetScreenWidth() - size - padding, padding, size, size};
            map = LoadRenderTexture(position.width, position.height);
        }
    }

    drawMinimap(playerPos, medium);
    check_map_update(playerPos);
}

void MiniMap::update(TileManager &tileManager, UI &ui, Vector2 playerPos, Texture2D &medium)
{
    camera.zoom = 1.5f;
    camera.offset = {position.width / 2, position.height / 2};
    camera.rotation = 180.0f;

    BeginTextureMode(map);
    ClearBackground(BLACK);

    BeginMode2D(camera);
    {
        draw(tileManager, ui, playerPos, medium);
    }
    EndMode2D();
    EndTextureMode();

    DrawTextureRec(map.texture, Rectangle{0.0f, 0.0f, -position.width, position.height},
                   Vector2{position.x, position.y}, WHITE);

    ui.miniMap(position);
}
