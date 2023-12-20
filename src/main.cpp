#include <iostream>   
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "raylib.h" 
#include "atlas.h" 
#include "player_camera.h"
#include "player.h" 
#include "tile_manager.h"
#include "tile_edit.h"
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

    TileChunk tileChunk(Vector2{0,0});
    TileChunk tileChunk2(Vector2{0,1});

    std::cout << sizeof(tileChunk)  << std::endl;

    while (!WindowShouldClose())     
    {  


        BeginDrawing();  


            BeginMode2D(camera.cam);

                ClearBackground(BLACK);            
                Vector2 mouseGridPos = getMouseGridPosition(camera.cam);


                
                /* Draw Tiles */  
                tileChunk.draw(atlas);
                tileChunk2.draw(atlas) ; 

                /* Draw Entities */ 
                player.update(atlas); 
                camera.update(player.physics_.pos);


                // DrawRectangleLines(player.physics_.pos.x + player. boundingRec.x, player.physics_.pos.y + player.boundingRec.y, player.boundingRec.width, player.boundingRec.height, BLACK);

                // std::cout << CheckCollisionRecs(Rectangle{player.physics_.pos.x + player.boundingRec.x, player.physics_.pos.y + player.boundingRec.y, player.boundingRec. width, player.boundingRec.height}, Rectangle{0,0, 16,16}) << std::endl;


            EndMode2D();
            DrawText("Delogy Pre-Alpha 1.0", 2, 0, 20, RAYWHITE);

            DrawFPS(2, 25); 
            std::string gridPosStr = std::to_string((int)mouseGridPos.x) + " " + std::to_string((int)mouseGridPos.y);
            DrawText(gridPosStr.c_str(), 2, 50, 20, DARKPURPLE);


            
        EndDrawing();   
    }          
    CloseWindow();         
}
