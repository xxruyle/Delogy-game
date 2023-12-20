#include <iostream>   
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "raylib.h" 
#include "atlas.h" 
#include "player_camera.h"
#include "player.h" 
#include "tile_manager.h"
#include "dev_util.h"
#include "FastNoiseLite.h"

#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576  

int main()    
{ 
    srand (time(NULL));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");   

    SetTargetFPS(60);     
    Atlas atlas("res/default_atlas.png");       
    PlayerCamera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    Player player(Vector2{0,0}, Rectangle{80, 0}, 4);

    // TileChunk tileChunk(Vector2{-3,-3});  
    // TileChunk tileChunk2(Vector2{0,0});  

    TileManager tileManager; 
    tileManager.generateChunks(); 


    // std::cout << Vector2Distance(Vector2{-8,-8}, Vector2{-10, -10}) << std::endl; 

    while (!WindowShouldClose())     
    {  


        BeginDrawing();  

            BeginMode2D(camera.cam);
                ClearBackground(BLACK);            
                Vector2 mouseGridPos = getMouseGridPosition(camera.cam);


                
                /* Draw Tile Chunks */  
                tileManager.drawAllChunks(atlas, player.physics_.pos);


                /* Draw Entities */ 
                player.update(atlas); 
                camera.update(player.physics_.pos);
            EndMode2D();




            DrawText("Delogy Pre-Alpha 1.0", 2, 0, 20, RAYWHITE);
            DrawFPS(2, 25); 

            std::string gridPosStr = std::to_string((int)mouseGridPos.x) + " " + std::to_string((int)mouseGridPos.y);
            DrawText(gridPosStr.c_str(), 2, 50, 20, DARKPURPLE);

            
        EndDrawing();   
    }          
    CloseWindow();         
}
