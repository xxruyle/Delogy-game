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

void MiniMap::getMapChangeInput()
{
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

    if (InputSystem::getUserMouseInteraction() == PLAYER_CREATE ||
        InputSystem::getUserMouseInteraction() == PLAYER_DESTROY) {
        updateMap = true;
    }
}

void MiniMap::drawMapChunks(TileManager &tileManager, Vector2 playerPos, Texture2D &medium)
{
    unsigned char alphaValue;
    float tileSize;
    if (fullScreen) {
        tileSize = 3.0f;
        alphaValue = 222;
        camera.zoom = 1.0f;
    }
    else {
        tileSize = 0.8f;
        alphaValue = 255;
        camera.zoom = 2.9f;
    }

    Vector2 playerGrid = {playerPos.x / 16 * tileSize, playerPos.y / 16 * tileSize};
    Rectangle playerMapIcon;
    if (fullScreen) {
        playerMapIcon = Rectangle{playerGrid.x - 8, playerGrid.y - 4, 16, 14};
    }
    else {
        playerMapIcon = Rectangle{playerGrid.x - 4, playerGrid.y - 2, 8, 7};
    }

    camera.target = playerGrid;

    std::vector<Vector2> chunkBuffer = tileManager.getNearbyChunks(playerPos, 3);

    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {

        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);

        std::vector<Vector2>::size_type index = tileManager.getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);
        visitedChunks[index] = 1;

        if (index < tileManager.chunks.size() && index >= 0 && tileManager.chunkExists(chunkBuffer[i])) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    int i = getIndex(x, y);
                    int id = tileManager.chunks[index].tileID[i];
                    int itemID = tileManager.chunks[index].itemID[i];

                    Vector2 loc;
                    loc.x = (float)((x + (tileManager.chunks[index].srcCoordinate.x * CHUNK_SIZE)) * tileSize);
                    loc.y = (float)((y + (tileManager.chunks[index].srcCoordinate.y * CHUNK_SIZE)) * tileSize);
                    Rectangle tileDest = {loc.x, loc.y, tileSize, tileSize};

                    if (id == WALL_FRONT) {
                        DrawRectangleRec(tileDest, Color{46, 34, 47, alphaValue});
                    }
                    else if (id == CAVE_FLOOR_MIDDLE) {
                        DrawRectangleRec(tileDest, Color{132, 145, 151, alphaValue});
                    }
                    else if (id == DIRT_FLOOR_MIDDLE) {
                        DrawRectangleRec(tileDest, Color{72, 48, 40, alphaValue});
                    }

                    if (itemID >= RAIL_NW && itemID <= RAIL_SW) {
                        DrawRectangleRec(tileDest, Color{74, 79, 86, alphaValue});
                    }

                    if (itemID == STORAGE_BOX) {
                        DrawRectangleRec(tileDest, Color{171, 148, 122, alphaValue});
                    }
                }
            }
        }
    }

    /* int count = 0; */
    /* for (int i = 0; i < WORLD_SIZE * 2 * WORLD_SIZE * 2; i++) { */
    /**/
    /*     if (visitedChunks[i] == 1) { */
    /*         if (i < tileManager.chunks.size() && i >= 0) { */
    /*             count++; */
    /*         } */
    /*     } */
    /* } */
    /**/
    /* std::cout << count << std::endl; */
    DrawTexturePro(medium, Rectangle{12, 167, 16.0f, 14.0f}, playerMapIcon, {0, 0}, 0.0f, WHITE);
}

void MiniMap::update(TileManager &tileManager, UI &ui, Vector2 playerPos, Texture2D &medium)
{
    camera.offset = {position.width / 2, position.height / 2};
    camera.rotation = 180.0f;

    getMapChangeInput();
    BeginTextureMode(map);
    ClearBackground(Color{0, 0, 0, 222}); // BLACK

    BeginMode2D(camera);
    {
        drawMapChunks(tileManager, playerPos, medium);
    }
    EndMode2D();
    EndTextureMode();

    DrawTextureRec(map.texture, Rectangle{0.0f, 0.0f, -position.width, position.height},
                   Vector2{position.x, position.y}, WHITE);

    ui.miniMap(position);
}
