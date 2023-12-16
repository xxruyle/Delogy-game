#include <iostream>   
#include "raylib.h" 
#include "atlas.h" 
#include "player_camera.h"
#include "player.h" 
#include "tile_manager.h"

#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576  

int main()    
{ 
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RGFXInProd");   

    // SetTargetFPS(60);   
    Atlas atlas("res/default_atlas.png"); 
    PlayerCamera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    Player player(Vector2{0,0}, Rectangle{0, 0}, 5);

    TileChunk tileChunk;
    tileChunk.fillTiles();
    
    while (!WindowShouldClose())     
    {  
        BeginDrawing();  
            BeginMode2D(camera.cam);
                ClearBackground(BLACK);            
                /* Draw Tiles */  
                tileChunk.draw(atlas); 

                /* Draw Entities */ 
                player.update(atlas); 
                camera.update(Vector2{0,0});

                DrawText("RGFXInProd", WINDOW_WIDTH / 2, WINDOW_HEIGHT /2, 11, GRAY);
            EndMode2D();
            DrawFPS(0, 0); 
            
        EndDrawing();   
    }          
    CloseWindow();         
}
