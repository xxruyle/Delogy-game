#include "tile_manager.h"



int TileChunk::getIndex(int i, int j)  
{
    return (row * i) + j * stride;
}; 

void TileChunk::fillTiles()
{

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {
            int index = getIndex(i, j);
            int x = index + 1;
            int y = index + 2;

            tiles[index] = TILE_COAL1.id;
            tiles[x] = TILE_COAL1.x;
            tiles[y] = TILE_COAL1.y; 
        }
    }
}


void TileChunk::draw(Atlas& atlas) 
{
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {
            int index = getIndex(i, j); 
            float x = tiles[index + 1];
            float y = tiles[index + 2];

            DrawTextureRec(atlas.texture, Rectangle{x, y, 16, 16}, Vector2{(float)(i*16), (float)(j * 16)}, WHITE);
        }
    }


}


TileManager::TileManager()
{


}

