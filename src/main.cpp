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

    TileManager tileManager; 
    tileManager.generateChunks(); 

    while (!WindowShouldClose())     
    {  
        BeginDrawing();  

            BeginMode2D(camera.cam);
                ClearBackground(BLACK);            
                
                /* Draw Tile Chunks */  
                tileManager.drawAllChunks(atlas, player.physics_.pos);

                /* Draw Entities */ 
                player.update(atlas); 
                camera.update(player.physics_.pos);
            EndMode2D();

            drawGameInfo();
            drawMouseChunkPosition(camera.cam);

        EndDrawing();   
    }          
    CloseWindow();         
}
