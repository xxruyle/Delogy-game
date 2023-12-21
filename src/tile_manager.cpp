#include "tile_manager.h"

int getIndex(int i, int j)   
{
    return (CHUNK_SIZE * DATA_STRIDE * i) + j * DATA_STRIDE;
};


void TileChunk::generateNoise() 
{
    const int xs = CHUNK_SIZE; 
    const int ys = CHUNK_SIZE;

    FastNoiseLite caveNoise; 
    caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    caveNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    caveNoise.SetSeed(rand() % 3000 + 1);
    caveNoise.SetFrequency(.1);

    for (int x = 0; x < xs; x++) 
    {
        for (int y = 0; y < ys; y++)  
        {
            float val = caveNoise.GetNoise((float)(srcCoordinate.x + x), (float)(srcCoordinate.y + y));

            int index = getIndex(y, x);

            if (val > .0017) 
            {
                tiles[index] = TILE_FLOOR_MIDDLE.id;
                tiles[index+1] = TILE_FLOOR_MIDDLE.x; 
                tiles[index+2] = TILE_FLOOR_MIDDLE.y;            
            } else { 
                tiles[index] = TILE_WALL_FRONT.id;
                tiles[index+1] = TILE_WALL_FRONT.x; 
                tiles[index+2] = TILE_WALL_FRONT.y;                
            }
        }
    }


}


void TileChunk::fillTiles() 
{

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {
            int index = getIndex(i, j);
            int x = index + 1;
            int y = index + 2;


            if (i == 0 && j == 0) {
                tiles[index] = TILE_WALL_NW.id;
                tiles[x] = TILE_WALL_NW.x;
                tiles[y] = TILE_WALL_NW.y;   
            } else if (i == 0 && j == 9) {           
                tiles[index] = TILE_WALL_NE.id;
                tiles[x] = TILE_WALL_NE.x;
                tiles[y] = TILE_WALL_NE.y;                 
            } else if (i == 1 && j > 0 && j < 9) { 
                tiles[index] = TILE_WALL_FRONT.id;
                tiles[x] = TILE_WALL_FRONT.x;
                tiles[y] = TILE_WALL_FRONT.y;           
            } else if (i == 8 && j == 1) {  
                tiles[index] = TILE_FLOOR_SW.id;
                tiles[x] = TILE_FLOOR_SW.x;
                tiles[y] = TILE_FLOOR_SW.y; 
            } else if (i > 2 && i < 9 && j == 1) { 
                tiles[index] = TILE_FLOOR_W.id;
                tiles[x] = TILE_FLOOR_W.x;
                tiles[y] = TILE_FLOOR_W.y;  
  
            } else if (i == 8 && j == 8) {  
                tiles[index] = TILE_FLOOR_SE.id;
                tiles[x] = TILE_FLOOR_SE.x;
                tiles[y] = TILE_FLOOR_SE.y;  
            } else if (i > 2 && i < 9 && j == 8) { 
                tiles[index] = TILE_FLOOR_E.id;
                tiles[x] = TILE_FLOOR_E.x;
                tiles[y] = TILE_FLOOR_E.y;  
            } else if (i == 2 && j == 1) {  
                tiles[index] = TILE_FLOOR_NW.id;
                tiles[x] = TILE_FLOOR_NW.x;
                tiles[y] = TILE_FLOOR_NW.y;                
            } else if (i == 2 && j == 8) {  
                tiles[index] = TILE_FLOOR_NE.id;
                tiles[x] = TILE_FLOOR_NE.x;
                tiles[y] = TILE_FLOOR_NE.y; 
            } else if (i == 2 && j > 1 && j < 8) {  
                tiles[index] = TILE_FLOOR_N.id;
                tiles[x] = TILE_FLOOR_N.x;
                tiles[y] = TILE_FLOOR_N.y;     
              
            } else if (i == 8 && j > 1 && j < 8) {  
                tiles[index] = TILE_FLOOR_S.id;
                tiles[x] = TILE_FLOOR_S.x;
                tiles[y] = TILE_FLOOR_S.y;                    
            } else if (i == 0 && j > 0 && j < 9) {
                tiles[index] = TILE_WALL_N.id;
                tiles[x] = TILE_WALL_N.x;
                tiles[y] = TILE_WALL_N.y;  
            } else if (i == 9 && j > 0 && j < 9) {
                tiles[index] = TILE_WALL_S.id;
                tiles[x] = TILE_WALL_S.x;
                tiles[y] = TILE_WALL_S.y;  
            } else if (i == 9 && j == 0) {
                tiles[index] = TILE_WALL_SW.id;
                tiles[x] = TILE_WALL_SW.x;
                tiles[y] = TILE_WALL_SW.y;  
            } else if (i == 9 && j == 9) {
                tiles[index] = TILE_WALL_SE.id;
                tiles[x] = TILE_WALL_SE.x;
                tiles[y] = TILE_WALL_SE.y; 
            } else if (i > 0 && j == 0) { 
                tiles[index] = TILE_WALL_W.id;
                tiles[x] = TILE_WALL_W.x;
                tiles[y] = TILE_WALL_W.y;                 
            } else if (i > 0 && j == 9) {
                tiles[index] = TILE_WALL_E.id;
                tiles[x] = TILE_WALL_E.x;
                tiles[y] = TILE_WALL_E.y;                  
            } else {
                tiles[index] = TILE_FLOOR_MIDDLE.id;
                tiles[x] = TILE_FLOOR_MIDDLE.x;
                tiles[y] = TILE_FLOOR_MIDDLE.y;                   
            }
        }
    }
}


void TileChunk::draw(Atlas& atlas) 
{
    if (IsKeyPressed(KEY_G)) 
    {
        generateNoise();
    }


    for (int i = 0; i < CHUNK_SIZE; i++) 
    {
        for (int j = 0; j < CHUNK_SIZE; j++) 
        {
            int index = getIndex(i, j);  
            float x = tiles[index + 1];
            float y = tiles[index + 2];

            DrawTextureRec(atlas.texture, Rectangle{x, y, 16, 16}, Vector2{(float)((j + (srcCoordinate.x * CHUNK_SIZE))*16), (float)((i+(srcCoordinate.y * CHUNK_SIZE)) * 16)}, WHITE);
        }
    }
}



void TileManager::generateChunks()
{
    for (int i = -WORLD_SIZE; i < WORLD_SIZE; i++) 
    {
        for (int j = -WORLD_SIZE; j < WORLD_SIZE; j++)
        {
            Vector2 chunkCoord = {(float)j, (float)i};

            TileChunk chunk(chunkCoord);
            chunks.push_back(chunk); 
        }
    }
}

bool TileManager::chunkExists(Vector2 chunkPos)
{
    return (chunkPos.x >= -WORLD_SIZE && chunkPos.x < WORLD_SIZE) && (chunkPos.y >= -WORLD_SIZE && chunkPos.y < WORLD_SIZE);
}

int TileManager::getChunkIndex(int x, int y)
{
    int stride = (WORLD_SIZE * 2 - 1);
    x = WORLD_SIZE + x; 
    y = WORLD_SIZE + y;
    return x + (y * (stride + 1)); 
}

void TileManager::drawAllChunks(Atlas& atlas, Vector2 playerPos)
{
    std::vector<Vector2> chunkBuffer = generateNearbyChunks(playerPos);
    
    for (std::vector<Vector2>::size_type i = 0; i < chunkBuffer.size(); i++)
    {
        Vector2 chunkPos = chunkToWorldSpace(chunkBuffer[i], CHUNK_SIZE);

        std::vector<Vector2>::size_type index = getChunkIndex(chunkBuffer[i].x, chunkBuffer[i].y);

        if (index < chunks.size() && index >= 0 && chunkExists(chunkBuffer[i]))
        {
            if (Vector2Distance(chunkPos, playerPos) < renderDistance * CHUNK_SIZE * 16)
            {
                chunks[index].draw(atlas);

                // For debugging 
                DrawText(std::to_string(index).c_str(),chunkPos.x, chunkPos.y, 40, RED); 
                DrawRectangleLines(chunkPos.x, chunkPos.y, CHUNK_SIZE * 16, CHUNK_SIZE * 16, WHITE);
            }
        }
    }
}

std::vector<Vector2> TileManager::generateNearbyChunks(Vector2 playerPos)
{
    playerPos = getGridPosition(playerPos);
    playerPos = getChunkPosition(playerPos, CHUNK_SIZE); // convert player absolute position to chunk position
    std::vector<Vector2> chunkBuffer;

    for (int i = -renderDistance; i < renderDistance; i++)
    {
        for (int j = -renderDistance; j < renderDistance; j++)
        {
            Vector2 nearChunk = {playerPos.x + j, playerPos.y + i};
            chunkBuffer.push_back(nearChunk);
        }
    }
    return chunkBuffer;
}

