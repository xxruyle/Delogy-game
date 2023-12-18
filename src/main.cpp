#include <iostream>   
#include "raylib.h" 
#include "atlas.h" 
#include "player_camera.h"
#include "player.h" 
#include "tile_manager.h"
#include "tile_edit.h"

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

                ClearBackground(WHITE);            

                
                /* Draw Tiles */  
                tileChunk.draw(atlas); 

                /* Draw Entities */ 
                player.update(atlas); 
                camera.update(player.physics_.pos);

                DrawText("RGFXInProd", WINDOW_WIDTH / 2, WINDOW_HEIGHT /2, 11, GRAY);

                drawMouseGridOutline(camera.cam);
                DrawRectangleLines(player.physics_.pos.x + player.boundingRec.x, player.physics_.pos.y + player.boundingRec.y, player.boundingRec.width, player.boundingRec.height, BLACK);

                // std::cout << CheckCollisionRecs(Rectangle{player.physics_.pos.x + player.boundingRec.x, player.physics_.pos.y + player.boundingRec.y, player.boundingRec. width, player.boundingRec.height}, Rectangle{0,0, 16,16}) << std::endl;






            EndMode2D();
            DrawFPS(0, 0); 
            
        EndDrawing();   
    }          
    CloseWindow();         
}
