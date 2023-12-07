#include "player.h"

PlayerAnimation::PlayerAnimation(Rectangle src, int animationFrames) 
    : frameAmount(animationFrames) 
{
    for (unsigned int i = 0; i < animationCount - 1; i++) {      
        animationSrcs[i] = (Rectangle{(float)(src.x), (float)(src.y + i * 16), (float)16, (float)16});           
    }
}



void PlayerAnimation::update()
{
    timeSinceLastFrameSwap += GetFrameTime();
    if (timeSinceLastFrameSwap > animationUpdateTime)  
    {
        curFrame < frameAmount - 1 ? curFrame++ : curFrame = 0; // increment curFrame after certain amount of time
        frameDelay = 0; 
        timeSinceLastFrameSwap = 0.0f;
    }

    // if (IsKeyDown(KEY_UP)) curAnimation = UP;  
    // if (IsKeyDown(KEY_RIGHT)) curAnimation = RIGHT;    
}

Player::Player(Rectangle src, int animationFrames) 
    : animation_(src, animationFrames)  
{
    
}


void Player::update(Atlas& atlas) 
{
    animation_.update();


    Rectangle curRec = animation_.animationSrcs[animation_.curAnimation]; 
    curRec.x = curRec.x + animation_.curFrame * 16; 

    DrawTextureRec(atlas.texture, curRec, Vector2{0, 0}, WHITE);
}