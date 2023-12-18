#pragma once  
#include <vector> 
#include <iostream>
#include "raylib.h" 
#include "raymath.h" 

#include "atlas.h"

enum animationDir {RIGHT, LEFT, UP, DOWN};   
enum stateType {MOVING, IDLE};

struct PlayerState 
{
    enum stateType curState = IDLE;
};

struct PlayerPhysics   
{
    Vector2 pos;  
    float velocity;
    float acceleration;

    PlayerPhysics(Vector2 spawnPos) : pos(spawnPos) {}; 
    void update(); 
 };  


struct PlayerAnimation  
{
    enum animationDir curAnimation = DOWN;  

    Rectangle curRec;
    Rectangle animationSrcs[4]; // right, left, up, down 
    int curFrames[4]; // the curFrames of each animation rec  
    int frameAmount;   
    
    float animationSpeed = 5.0f;
    float animationUpdateTime = 1.0f / animationSpeed;
    float timeSinceLastFrameSwap = 0.0f;

    PlayerAnimation(Rectangle src, int animationFrames);    

    void update(PlayerState& state);
    void changeAnimation(PlayerState& state); 
};


struct PlayerInput 
{
    void getInput(PlayerPhysics& physics, PlayerAnimation& animation, PlayerState& state);
    void resetInput(PlayerAnimation& animation, PlayerState& state);

    void update(PlayerPhysics& physics, PlayerAnimation& animation, PlayerState& state); 
};


struct Player  
{
    bool isMoving; 
    Player(Vector2 spawnPos, Rectangle src, int animationFrames);  
    PlayerAnimation animation_; 
    PlayerPhysics physics_; 
    PlayerInput input_; 
    PlayerState state_; 

    Rectangle boundingRec = Rectangle{2, 1, 12, 15};

    void update(Atlas& atlas); 
};