#include "tile_manager.hpp"
#include "FastNoiseLite.h"
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
                tileID[index] = TILE_WALL_FRONT.id;
                tileZ[index] = 1;
            }
            else {
                tileID[index] = TILE_CAVE_FLOOR_MIDDLE.id;
                tileZ[index] = 0;
            }
        }
    }
}

void TileChunk::draw(Atlas &atlas)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            drawTile(atlas, x, y);
            drawItem(atlas, x, y);
        }
    }
}

void TileChunk::drawTile(Atlas &atlas, int x, int y)
{
    int index = getIndex(x, y);
    int id = tileID[index];
    Tile curTile = tileids[id];
    float xAtlasPos = curTile.x;
    float yAtlasPos = curTile.y;

    /* DrawTextureRec(atlas.texture, Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f}, */
    /*                Vector2{(float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * (float)16), */
    /*                        (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * (float)16)}, */
    /*                WHITE); // casting to float helps with texture coordinate offset with camera movement */

    Rectangle tileAtlasPos = Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f};
    Vector2 loc;
    loc.x = (float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * 16.0f);
    loc.y = (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * 16.0f);
    Rectangle tileDest = {loc.x, loc.y, 16.0f, 16.0f};
    DrawTexturePro(atlas.texture, tileAtlasPos, tileDest, {0, 0}, 0.0f, WHITE);
}

void TileChunk::drawItem(Atlas &atlas, int x, int y)
{
    int index = getIndex(x, y);
    int itemid = itemID[index];
    if (itemid != 0) // if an item exists here
    {
        Item curItem = itemids[itemid];
        float xAtlasPos = curItem.x;
        float yAtlasPos = curItem.y;
        /* DrawTextureRec(atlas.texture, Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f}, */
        /*                Vector2{(float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * (float)16), */
        /*                        (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * (float)16)}, */
        /*                WHITE); // casting to float helps with texture coordinate integer offset with camera movement
         */
        Rectangle tileAtlasPos = Rectangle{xAtlasPos, yAtlasPos, 16.0f, 16.0f};
        Vector2 loc;
        loc.x = (float)(((float)x + ((float)srcCoordinate.x * (float)CHUNK_SIZE)) * 16.0f);
        loc.y = (float)(((float)y + ((float)srcCoordinate.y * (float)CHUNK_SIZE)) * 16.0f);
        Rectangle tileDest = {loc.x, loc.y, 16.0f, 16.0f};
        DrawTexturePro(atlas.texture, tileAtlasPos, tileDest, {0, 0}, 0.0f, WHITE);
    }
}

void TileChunk::deleteAtTile(int x, int y)
{
    int index = getIndex(x, y);
    if (itemID[index] != 0) { // if an item exists at this spot
        itemID[index] = 0;
    }
    else {

        int zLevel = tileZ[index]; // only delete the tile if z level is > 0
        if (zLevel > 0) {
            tileID[index] = TILE_CAVE_FLOOR_MIDDLE.id;
            tileZ[index] = 0;
        }
        else {
            /* std::cout << "Lowest layer!" << std::endl; */
        }
    }
}

void TileChunk::updateTile(int x, int y)
{
    int index = getIndex(x, y);
    tileID[index] = TILE_COAL1.id;
    // we don't need to change z level because we are just replacing the highest block
}

void TileChunk::updateItem(int x, int y, int playerItemID)
{
    int index = getIndex(x, y);
    int curItemID = itemID[index];
    Item newItem = itemids[playerItemID];

    if (curItemID == 0) { // if there is no existing item here
        itemID[index] = newItem.id;
    }
    else {
        /* std::cout << curItemID << std::endl; */
    }
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

void TileManager::checkPlayerInteraction(Player &player, UI &ui)
{
    if (player.state_.curState == INTERACTING) {
        Vector2 mousePos = getMouseGridPosition(player.camera_.cam);
        Vector2 chunkPos = getMouseChunkPosition(player.camera_.cam);
        int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y);
        Vector2 relativeGridPos = getRelativeChunkGridPosition(chunkPos, mousePos);
        if (chunkExists(chunkPos) &&
            ui.mouseOutOfBounds()) { // player can interact if chunk exists and mouse is not over ui
            switch (player.state_.curAction) {
            case DESTROY: {
                chunks[chunkIndex].deleteAtTile(relativeGridPos.x, relativeGridPos.y);
                break;
            }
            case CREATE: {
                int curPlayerItemIndex = player.inventory_.curHotbarItem;
                chunks[chunkIndex].updateItem(relativeGridPos.x, relativeGridPos.y,
                                              player.inventory_.itemHotbar[curPlayerItemIndex]);
                break;
            }
            default:
                break;
            }
        }
    }
    player.state_.curAction = NORMAL; // resetting player state
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

void TileManager::drawAllChunks(Atlas &atlas, Vector2 &playerPos)
{
    std::vector<Vector2> chunkBuffer = generateNearbyChunks(playerPos);

    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++) {
        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i]);

        std::vector<Vector2>::size_type index = getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

        if (index < chunks.size() && index >= 0 && chunkExists(chunkBuffer[i])) {
            if (Vector2Distance(chunkPos, playerPos) < RENDER_DISTANCE * CHUNK_SIZE * 16) {
                chunks[index].draw(atlas);
                /* drawChunkInfo(chunkPos); */
            }
        }
    }
}

void TileManager::update(Atlas &atlas, Player &player, UI &ui)
{
    drawAllChunks(atlas, player.physics_.pos);
    checkPlayerInteraction(player, ui);
}
