#include <iostream>  
#include "raylib.h" 
#include "atlas.h" 
#include "player_camera.h"
#include "player.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080  

int main()  
{ 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RGFXInProd");  

    Atlas atlas("res/default_atlas.png");
    PlayerCamera camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    Player player(Rectangle{0, 0}, 5);


    while (!WindowShouldClose())   
    {  
        BeginDrawing();  
            BeginMode2D(camera.cam);

                ClearBackground(RAYWHITE);          
                camera.update(Vector2{0, 0});
                player.update(atlas);
                DrawText("RGFXInProd", WINDOW_WIDTH / 2, WINDOW_HEIGHT /2, 11, GRAY);

            EndMode2D();
            DrawFPS(0, 0); 
            
        EndDrawing();   
    }          
    CloseWindow();         
}
