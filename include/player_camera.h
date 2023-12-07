#pragma once 
#include "raylib.h"  
#include "raymath.h" 
#include <iostream>

struct PlayerCamera 
{
    Camera2D cam = {0};
    Vector2 playerPos; 
    float windowWidth; 
    float windowHeight;
    bool freeCam = false;


    PlayerCamera(int window_width, int window_height); 

    void update(Vector2 target);
    void updateTarget(Vector2 target);
    void updateZoom();
    void updateFreeCam();  
}; 