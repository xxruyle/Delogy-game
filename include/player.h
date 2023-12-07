#pragma once  
#include <vector> 
#include <iostream>

#include "raylib.h" 
#include "raymath.h" 

#include "atlas.h"


struct PlayerPhysics
{
    Vector2 pos; 
    void update(); 
};  

enum animationDir {RIGHT, LEFT, UP, DOWN, numberOfAnimations};   

struct PlayerAnimation 
{
    enum animationDir animationCount = numberOfAnimations; // the number of animations in animationDir
    enum animationDir curAnimation = RIGHT; 
    Rectangle animationSrcs[4]; // right, left, up, down 

    int frameAmount;   
    int curFrame = 0;
    int frameDelay = 0;


    float animationSpeed = 5.0f;
    float animationUpdateTime = 1.0f / animationSpeed;
    float timeSinceLastFrameSwap = 0.0f;


    PlayerAnimation(Rectangle src, int animationFrames);   

    void update();
};

struct Player  
{
    Player(Rectangle src, int animationFrames); 
    PlayerAnimation animation_;

    void update(Atlas& atlas);

};