#include "tile_manager.hpp"

#include "dev_util.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "macros_util.hpp"
#include "tile_data.hpp"
#include "FastNoiseLite.h"

int getIndex(int x, int y) { return (CHUNK_SIZE * y) + x; };

/* ---------------- TileChunk Methods  ----------------*/
void TileChunk::wallGeneration()
{
    // fill chunk with wall tiles
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int index = getIndex(x, y);
            tileID[index] = TILE_WALL_FRONT.id;
            tileZ[index] = 1;
        }
    }
}

void TileChunk::generateNoise(int seed)
{
    const int xs = CHUNK_SIZE;
    const int ys = CHUNK_SIZE;

    FastNoiseLite caveNoise;
    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    caveNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    caveNoise.SetFrequency(.001);
    caveNoise.SetFractalOctaves(4);
    caveNoise.SetSeed(seed);
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

void TileChunk::drunkardWalk()
{

    enum cardinalDirection { NORTH, EAST, SOUTH, WEST };

    int floorCount = 0;
    Vector2 curTile = {15, 15};
    while (floorCount < 500) {
        if (curTile.x >= 0 && curTile.y >= 0 && curTile.y < 32 && curTile.x < 32) {
            int index = getIndex(curTile.x, curTile.y);
            if (tileID[index] != DIRT_FLOOR_MIDDLE) {
                tileID[index] = TILE_DIRT_FLOOR_MIDDLE.id;
                tileZ[index] = 0;
                floorCount++;
            }
        }
        else {
            bool foundNew = false;
            while (!foundNew) {
                Vector2 randomSpot = Vector2{GetRandomValue(0, 32), GetRandomValue(0, 32)};
                int index = getIndex(randomSpot.x, randomSpot.y);
                if (tileID[index] == TILE_DIRT_FLOOR_MIDDLE.id) {
                    foundNew = true;
                    curTile = randomSpot;
                }
            }
        }

        int randomDirection = GetRandomValue(0, 3);
        switch (randomDirection) {
        case NORTH:
            curTile.y -= 1;
            break;
        case EAST:
            curTile.x += 1;
            break;
        case SOUTH:
            curTile.y += 1;
            break;
        case WEST:
            curTile.x -= 1;
            break;
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
            tileID[index] = TILE_DIRT_FLOOR_MIDDLE.id;
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
    tileID[index] = TILE_CAVE_FLOOR_MIDDLE.id;
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

    drunkardGenerateAll();
}

bool TileManager::chunkExists(Vector2 chunkPos)
{
    return (chunkPos.x >= -WORLD_SIZE && chunkPos.x < WORLD_SIZE) &&
           (chunkPos.y >= -WORLD_SIZE && chunkPos.y < WORLD_SIZE);
}

int TileManager::getChunkIndex(int x, int y)
{ // given the coordinate of a chunk
    int stride = (WORLD_SIZE * 2 - 1);
    x = WORLD_SIZE + x;
    y = WORLD_SIZE + y;
    return x + (y * (stride + 1));
}

void TileManager::checkPlayerInteraction(Camera2D &camera, UI &ui, InventoryC &playerInventory)
{
    int interactKey = InputSystem::getUserMouseInteraction();
    if (interactKey) {
        Vector2 mousePos = getMouseGridPosition(camera);
        Vector2 chunkPos = getMouseChunkPosition(camera);

        int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y);
        Vector2 relativeGridPos = getRelativeChunkGridPosition(chunkPos, mousePos);

        if (chunkExists(chunkPos) &&
            ui.mouseOutOfBounds()) { // player can interact if chunk exists and mouse is not over ui
            switch (interactKey) {
            case PLAYER_DESTROY: {
                chunks[chunkIndex].deleteAtTile(relativeGridPos.x, relativeGridPos.y);
                break;
            }
            case PLAYER_CREATE: {
                int selectedItem = playerInventory.hotbar[playerInventory.curItem];
                if (selectedItem >= RAIL_NW && selectedItem < CART) {
                    chunks[chunkIndex].updateItem(relativeGridPos.x, relativeGridPos.y, selectedItem);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

std::vector<Vector2> TileManager::getNearbyChunks(Vector2 playerPos)
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
    std::vector<Vector2> chunkBuffer = getNearbyChunks(playerPos);

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

void TileManager::update(Atlas &atlas, UI &ui, Scene &scene)
{

    PositionC &position = scene.EntityRegistry.get<PositionC>(scene.player);
    InventoryC &inventory = scene.EntityRegistry.get<InventoryC>(scene.player);
    drawAllChunks(atlas, position.pos);
    checkPlayerInteraction(scene.camera, ui, inventory);
}

int TileManager::getItemUnder(Vector2 pos)
{
    Vector2 centeredPos = {pos.x, pos.y};
    Vector2 gridPos = getGridPosition(centeredPos);
    Vector2 chunkPos = getChunkPosition(gridPos);
    std::vector<Vector2>::size_type index = getChunkIndex(chunkPos.x, chunkPos.y);
    Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, gridPos);
    int relativeChunkIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y);

    /* IndexPair indexPair = getIndexPair((int)pos.x, (int)pos.y); */
    /* std::cout << indexPair.x << " " << indexPair.y << " | " << index << " " << relativeChunkIndex << std::endl; */

    /* if (indexPair.x != index || indexPair.y != relativeChunkIndex) { */
    /*     std::cout << indexPair.x << " " << indexPair.y << " | " << index << " " << relativeChunkIndex << std::endl;
     */
    /* } */

    return chunks[index].itemID[relativeChunkIndex];
}

std::vector<Vector2> TileManager::getNeighbors(int x, int y, int radius)
{
    std::vector<Vector2> neighbors; // pairs of chunk index and relative chunk index

    for (int row = y - radius; row < y + radius; row++) {
        for (int col = x - radius; col < x + radius; col++) {
            if (isValidCoordinate(col, row)) {
                neighbors.push_back(Vector2{col, row});
                /* std::cout << col << " " << row << std::endl; */
            }
        }
    }
    /* std::cout << std::endl; */

    return neighbors;
}

bool TileManager::isValidCoordinate(int x, int y)
{
    int worldBound = CHUNK_SIZE * WORLD_SIZE;
    return ((x >= -worldBound && y >= -worldBound) && (x < worldBound && y < worldBound));
}

IndexPair TileManager::getIndexPair(int x, int y)
{
    /* Vector2 gridPos = getGridPosition(Vector2{x, y}); */
    /* Vector2 chunkPos = getChunkPosition(gridPos); */
    /* int chunkIndex = getChunkIndex(chunkPos.x, chunkPos.y); */
    /* Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, gridPos); */
    /* int relativeChunkIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y); */

    Vector2 centeredPos = {x, y};
    Vector2 gridPos = getGridPosition(centeredPos);
    Vector2 chunkPos = getChunkPosition(gridPos);
    int index = getChunkIndex(chunkPos.x, chunkPos.y);
    Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, gridPos);
    int relativeChunkIndex = getIndex((int)relativeChunkGridPos.x, (int)relativeChunkGridPos.y);

    return IndexPair{index, relativeChunkIndex};
}

void TileManager::drunkardGenerateAll()
{
    enum cardinalDirection { NORTH, EAST, SOUTH, WEST };

    int floorCount = 0;
    int worldLength = CHUNK_SIZE * WORLD_SIZE;
    int totalTileCount = worldLength * worldLength;
    Vector2 curTile = {0, 0};
    while (floorCount < totalTileCount / 1.5f) {

        /* if (floorCount % 13 == 0) { */
        /*     std::cout << floorCount << " " << totalTileCount / 1.5f << std::endl; */
        /* } */
        Vector2 chunkPos = getChunkPosition(curTile);
        int chunkIndex = getChunkIndex((int)chunkPos.x, (int)chunkPos.y);
        Vector2 relativeChunkGridPos = getRelativeChunkGridPosition(chunkPos, curTile);
        int tileIndex = getIndex(relativeChunkGridPos.x, relativeChunkGridPos.y);

        if (curTile.x >= -worldLength && curTile.y >= -worldLength && curTile.y < worldLength &&
            curTile.x < worldLength) {

            if (chunks[chunkIndex].tileID[tileIndex] != DIRT_FLOOR_MIDDLE) {
                chunks[chunkIndex].tileID[tileIndex] = TILE_DIRT_FLOOR_MIDDLE.id;
                chunks[chunkIndex].tileZ[tileIndex] = 0;
                floorCount++;
            }
        }
        else {
            /* curTile = Vector2{GetRandomValue(-worldLength, worldLength), GetRandomValue(-worldLength, worldLength)};
             */
            curTile = Vector2{0, 0};
            /* bool foundNew = false; */
            /* while (!foundNew) { */
            /*     Vector2 randomSpot = */
            /*         Vector2{GetRandomValue(-worldLength, worldLength), GetRandomValue(-worldLength, worldLength)}; */
            /*     int index = getIndex(randomSpot.x, randomSpot.y); */
            /*     if (chunks[chunkIndex].tileID[tileIndex] == TILE_DIRT_FLOOR_MIDDLE.id) { */
            /*         foundNew = true; */
            /*         curTile = randomSpot; */
            /*     } */
            /* } */
        }

        int randomDirection = GetRandomValue(0, 3);
        switch (randomDirection) {
        case NORTH:
            curTile.y -= 1;
            break;
        case EAST:
            curTile.x += 1;
            break;
        case SOUTH:
            curTile.y += 1;
            break;
        case WEST:
            curTile.x -= 1;
            break;
        }
    }
}
