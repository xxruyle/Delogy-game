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

void MiniMap::getMapChangeInput(UI &ui, Vector2 playerPos)
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

            // reset the target and offset when switching (Added so player can't get lost)
            Vector2 playerGrid = {playerPos.x / 16 * fullScreenTileSize, playerPos.y / 16 * fullScreenTileSize};
            fullScreenTarget = playerGrid;
            fullScreenOffset = {position.width / 2, position.height / 2};
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), ui.bounds[MINIMAP])) {
        if (fullScreen) {
            fullScreenZoom += GetMouseWheelMove() * 0.2f;
            if (fullScreenZoom > 5.0f)
                fullScreenZoom = 5.0f;
            else if (fullScreenZoom < 0.5f)
                fullScreenZoom = 0.5f;

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 delta = Vector2Scale(GetMouseDelta(), -1.0f / fullScreenZoom);
                fullScreenTarget = Vector2Add(fullScreenTarget, delta);
                fullScreenOffset = {position.width / 2, position.height / 2};
            }
        }
        else {
            defaultZoom += GetMouseWheelMove() * 0.2f;

            if (defaultZoom > 10.0f)
                defaultZoom = 10.0f;
            else if (defaultZoom < 0.5f)
                defaultZoom = 0.5f;
        }
    }
}

void MiniMap::storeChunkTexture(TileManager &tileManager, int index)
{
    RenderTexture chunkTex = LoadRenderTexture(fullScreenTileSize * CHUNK_SIZE, fullScreenTileSize * CHUNK_SIZE);
    BeginTextureMode(chunkTex);
    if (index < tileManager.chunks.size() && index >= 0) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int i = getIndex(x, y);
                int id = tileManager.chunks[index].tileID[i];
                int itemID = tileManager.chunks[index].itemID[i];

                Vector2 loc;
                loc.x = (float)((x * fullScreenTileSize));
                loc.y = (float)((y * fullScreenTileSize));
                Rectangle tileDest = {loc.x, loc.y, fullScreenTileSize, fullScreenTileSize};

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
    EndTextureMode();
    mapChunks[index] = chunkTex;
}

void MiniMap::storeNewChunkTextures(TileManager &tileManager, Vector2 playerPos)
{
    std::vector<Vector2> chunkBuffer = tileManager.getNearbyChunks(playerPos, 3);

    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {
        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);
        std::vector<Vector2>::size_type index = tileManager.getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

        if (visitedChunks[index] == 0) {
            visitedChunks[index] = 1;
            storeChunkTexture(tileManager, index);
        }
    }
}

void MiniMap::drawNearChunks(TileManager &tileManager, std::vector<Vector2> &chunkBuffer)
{
    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {

        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);
        std::vector<Vector2>::size_type index = tileManager.getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

        if (index < tileManager.chunks.size() && index >= 0 && tileManager.chunkExists(chunkBuffer[i])) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    int i = getIndex(x, y);
                    int id = tileManager.chunks[index].tileID[i];
                    int itemID = tileManager.chunks[index].itemID[i];

                    Vector2 loc;
                    loc.x = (float)((x + (tileManager.chunks[index].srcCoordinate.x * CHUNK_SIZE)) * defaultTileSize);
                    loc.y = (float)((y + (tileManager.chunks[index].srcCoordinate.y * CHUNK_SIZE)) * defaultTileSize);
                    Rectangle tileDest = {loc.x, loc.y, defaultTileSize, defaultTileSize};

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
                        DrawLine(tileDest.x + 8, tileDest.y, tileDest.x, tileDest.height, BROWN);
                    }

                    if (itemID == STORAGE_BOX) {
                        DrawRectangleRec(tileDest, Color{171, 148, 122, alphaValue});
                    }
                }
            }
        }
    }
}

void MiniMap::drawVisitedChunks(TileManager &tileManager, std::vector<Vector2> &chunkBuffer)
{
    for (int index = 0; index < WORLD_SIZE * 2 * WORLD_SIZE * 2; index++) {
        if (visitedChunks[index] == 1) {
            Rectangle src = {0.0f, 0.0f, mapChunks[index].texture.width, -mapChunks[index].texture.height};
            Vector2 chunkSrc = {tileManager.chunks[index].srcCoordinate.x * CHUNK_SIZE * fullScreenTileSize,
                                tileManager.chunks[index].srcCoordinate.y * CHUNK_SIZE * fullScreenTileSize};
            DrawTextureRec(mapChunks[index].texture, src, chunkSrc, WHITE);
        }
    }
}

void MiniMap::updateChunks(TileManager &tileManager)
{
    if (tileManager.updatedChunks.size() > 0) {
        for (int index : tileManager.updatedChunks) {
            if (visitedChunks[index] == 1) {
                UnloadRenderTexture(mapChunks[index]);
                storeChunkTexture(tileManager, index);
            }
        }

        tileManager.updatedChunks.clear();
    }
}

void MiniMap::drawMapChunks(TileManager &tileManager, Vector2 playerPos, Texture2D &medium)
{
    Vector2 playerGrid;
    if (fullScreen) {
        playerGrid = {playerPos.x / 16 * fullScreenTileSize, playerPos.y / 16 * fullScreenTileSize};

        camera.zoom = fullScreenZoom;
        camera.target = fullScreenTarget;
        camera.offset = fullScreenOffset;
    }
    else {
        playerGrid = {playerPos.x / 16 * fullScreenTileSize, playerPos.y / 16 * fullScreenTileSize};
        camera.zoom = defaultZoom;
        camera.target = playerGrid;
        camera.offset = {position.width / 2, position.height / 2};
    }

    Rectangle playerMapIcon;
    if (fullScreen) {
        playerMapIcon = Rectangle{playerGrid.x - 8, playerGrid.y - 4, 16, 14};
    }
    else {
        playerMapIcon = Rectangle{playerGrid.x - 4, playerGrid.y - 2, 8, 7};
    }

    std::vector<Vector2> chunkBuffer = tileManager.getNearbyChunks(playerPos, 3);

    drawVisitedChunks(tileManager, chunkBuffer);

    // player icon
    DrawTexturePro(medium, Rectangle{12, 167, 16.0f, 14.0f}, playerMapIcon, {0, 0}, 0.0f, WHITE);
}

void MiniMap::draw(TileManager &tileManager, Vector2 playerPos, Texture2D &medium)
{
    BeginTextureMode(map);
    if (fullScreen) {
        ClearBackground(Color{0, 0, 0, fullScreenAlphaValue}); // BLACK_TRANSPARENT
    }
    else {
        ClearBackground(BLACK);
    }

    BeginMode2D(camera);
    {
        drawMapChunks(tileManager, playerPos, medium);
    }
    EndMode2D();
    EndTextureMode();

    DrawTextureRec(map.texture, Rectangle{0.0f, 0.0f, position.width, -position.height},
                   Vector2{position.x, position.y}, WHITE);
}

void MiniMap::update(TileManager &tileManager, UI &ui, Vector2 playerPos, Texture2D &medium)
{
    getMapChangeInput(ui, playerPos);

    draw(tileManager, playerPos, medium);

    ui.miniMap(position);

    storeNewChunkTextures(tileManager, playerPos);
    updateChunks(tileManager);
}

MiniMap::~MiniMap()
{
    UnloadTexture(map.texture);
    for (int i = 0; i < WORLD_SIZE * 2 * WORLD_SIZE * 2; i++) {
        if (visitedChunks[i] == 1) {
            UnloadRenderTexture(mapChunks[i]);
        }
    }
}
