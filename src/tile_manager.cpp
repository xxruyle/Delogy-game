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
            } else if (i > 2 && i < 9 && j == 1) { 
                tiles[index] = TILE_FLOOR_W.id;
                tiles[x] = TILE_FLOOR_W.x;
                tiles[y] = TILE_FLOOR_W.y;  
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
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++) 
        {
            int index = getIndex(i, j); 
            float x = tiles[index + 1];
            float y = tiles[index + 2];

            DrawTextureRec(atlas.texture, Rectangle{x, y, 16, 16}, Vector2{(float)(j*16), (float)(i * 16)}, WHITE);
        }
    }


}


TileManager::TileManager()
{


}

