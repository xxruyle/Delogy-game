#include "tile_manager.hpp"
#include "FastNoiseLite.h"
#include "dev_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include "tile_data.hpp"

int getIndex(int x, int y) { return (CHUNK_SIZE * y) + x; };

/* ---------------- TileChunk Methods  ----------------*/
void TileChunk::generateNoise(int seed)
{
    const int xs = CHUNK_SIZE;
    const int ys = CHUNK_SIZE;

    FastNoiseLite caveNoise;
    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    caveNoise.SetSeed(seed);
    caveNoise.SetFrequency(.0001);
    for (int y = 0; y < ys; y++) {
        for (int x = 0; x < xs; x++) {
            float val = caveNoise.GetNoise((float)(srcCoordinate.x * CHUNK_SIZE * 16 + x * 16),
                                           (float)(srcCoordinate.y * CHUNK_SIZE * 16 + y * 16));

            int index = getIndex(x, y);

            if (val > .0017) {
                tileID[index] = TILE_FLOOR_MIDDLE.id;
                tileX[index] = TILE_FLOOR_MIDDLE.x;
                tileY[index] = TILE_FLOOR_MIDDLE.y;
                tileZ[index] = 1;
            }
            else {
                tileID[index] = TILE_WALL_FRONT.id;
                tileX[index] = TILE_WALL_FRONT.x;
                tileY[index] = TILE_WALL_FRONT.y;
                tileZ[index] = 0;
            }
        }
    }
}

void TileChunk::draw(Atlas &atlas)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int index = getIndex(x, y);
            float xPos = tileX[index];
            float yPos = tileY[index];

            DrawTextureRec(atlas.texture, Rectangle{xPos, yPos, 16, 16},
                           Vector2{(float)((x + (srcCoordinate.x * CHUNK_SIZE)) * 16),
                                   (float)((y + (srcCoordinate.y * CHUNK_SIZE)) * 16)},
                           WHITE);
        }
    }
}

void TileChunk::drawEntities(Atlas &atlas) {}

void TileChunk::deleteTile(int x, int y)
{
    int index = getIndex(x, y);
    int id = tileID[index];
    Tile curTile = tileids[id]; // the current tile that we wan't to delete

    int zLevel = tileZ[index]; // only delete the tile if z level is > 0
    if (zLevel > 0) {
        tileID[index] = TILE_WALL_FRONT.id;
        tileX[index] = TILE_WALL_FRONT.x;
        tileY[index] = TILE_WALL_FRONT.y;
        tileZ[index] = 0;
    }
    else {
        std::cout << "Lowest layer!" << std::endl;
    }
}

void TileChunk::updateTile(int x, int y)
{
    int index = getIndex(x, y);
    int id = tileID[index];
    Tile curTile = tileids[id]; // the current tile that we wan't to delete

    tileID[index] = TILE_RAIL_V.id;
    tileX[index] = TILE_RAIL_V.x;
    tileY[index] = TILE_RAIL_V.y;
    tileZ[index] = 1;
}

/* ---------------- TileManager Methods  ----------------*/
void TileManager::generateChunks()
{
    for (int y = -WORLD_SIZE; y < WORLD_SIZE; y++) {
        for (int x = -WORLD_SIZE; x < WORLD_SIZE; x++) {
            Vector2 chunkCoord = {(float)x, (float)y};

            TileChunk chunk(chunkCoord, world_seed);
            chunks.push_back(chunk);
        }
    }
}

bool TileManager::chunkExists(Vector2 chunkPos)
{
    return (chunkPos.x >= -WORLD_SIZE && chunkPos.x < WORLD_SIZE) &&
           (chunkPos.y >= -WORLD_SIZE && chunkPos.y < WORLD_SIZE);
}

int TileManager::getChunkIndex(int x, int y)
{
    int stride = (WORLD_SIZE * 2 - 1);
    x = WORLD_SIZE + x;
    y = WORLD_SIZE + y;
    return x + (y * (stride + 1));
}

std::vector<Vector2> TileManager::generateNearbyChunks(Vector2 playerPos)
{
    playerPos = getGridPosition(playerPos);
    playerPos = getChunkPosition(playerPos); // convert player absolute position to chunk position
    std::vector<Vector2> chunkBuffer;

    for (int i = -RENDER_DISTANCE; i < RENDER_DISTANCE; i++) {
        for (int j = -RENDER_DISTANCE; j < RENDER_DISTANCE; j++) {
            Vector2 nearChunk = {playerPos.x + j, playerPos.y + i};
            chunkBuffer.push_back(nearChunk);
        }
    }
    return chunkBuffer;
}

void TileManager::checkPlayerInteraction(Player &player)
{
    if (player.state_.curState == INTERACTING) {
        Vector2 mousePos = getMouseGridPosition(player.camera_.cam);
        Vector2 chunkPos = getMouseChunkPosition(player.camera_.cam);
        int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y);
        Vector2 relativeGridPos = getRelativeChunkGridPosition(chunkPos, mousePos);
        if (chunkExists(chunkPos)) {
            switch (player.state_.curAction) {
            case DESTROY: {
                chunks[chunkIndex].deleteTile(relativeGridPos.x, relativeGridPos.y);
                break;
            }
            case CREATE: {
                chunks[chunkIndex].updateTile(relativeGridPos.x, relativeGridPos.y);
                break;
            }
            default:
                break;
            }
        }
    }
    player.state_.curAction = NORMAL; // resetting player state
}

void TileManager::drawAllChunks(Atlas &atlas, Vector2 &playerPos)
{
    std::vector<Vector2> chunkBuffer = generateNearbyChunks(playerPos);

    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {
        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);

        std::vector<Vector2>::size_type index = getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

        if (index < chunks.size() && index >= 0 && chunkExists(chunkBuffer[i])) {
            if (Vector2Distance(chunkPos, playerPos) < RENDER_DISTANCE * CHUNK_SIZE * 16) {
                chunks[index].draw(atlas);
                std::string pos =
                    "(" + std::to_string((int)chunkBuffer[i].x) + ", " + std::to_string((int)chunkBuffer[i].y) + ")";

                // For debugging
                DrawText(pos.c_str(), chunkPos.x, chunkPos.y, 40, RED);
                DrawRectangleLines(chunkPos.x, chunkPos.y, CHUNK_SIZE * 16, CHUNK_SIZE * 16, WHITE);
            }
        }
    }
}

void TileManager::update(Atlas &atlas, Player &player)
{
    drawAllChunks(atlas, player.physics_.pos);
    checkPlayerInteraction(player);
}
