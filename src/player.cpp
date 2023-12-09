#include "player.h"

void PlayerPhysics::update()
{
    
}


void PlayerInput::getInput(PlayerPhysics& physics, PlayerAnimation& animation, PlayerState& state)
{
    if (IsKeyDown(KEY_A)) // left
    {
        state.curState = MOVING;
        animation.curAnimation = LEFT;
    }

    if (IsKeyDown(KEY_D)) // right
    {
        state.curState = MOVING;
        animation.curAnimation = RIGHT;
    }


    if (IsKeyDown(KEY_W)) // up 
    {
        state.curState = MOVING;
        animation.curAnimation = UP;
    }

    if (IsKeyDown(KEY_S)) // down 
    {
        state.curState = MOVING;
        animation.curAnimation = DOWN;
    }
}

void PlayerInput::resetInput(PlayerAnimation& animation, PlayerState& state)
{
    if (!IsKeyDown(KEY_A)) // LEFT
    {
        animation.curFrames[LEFT] = 0;
    }

    if (!IsKeyDown(KEY_D)) // RIGHT
    {
        animation.curFrames[RIGHT] = 0;
    }


    if (!IsKeyDown(KEY_W)) // up 
    {
        animation.curFrames[UP] = 0;
    }

    if (!IsKeyDown(KEY_S)) // down 
    {
        animation.curFrames[DOWN] = 0;
    }


    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
    {
        state.curState = IDLE;
    }
}

void PlayerInput::update(PlayerPhysics& physics, PlayerAnimation& animation, PlayerState& state)
{
    resetInput(animation, state);
    getInput(physics, animation, state);
}


PlayerAnimation::PlayerAnimation(Rectangle src, int animationFrames) 
    : frameAmount(animationFrames) 
{
    for (unsigned int i = 0; i < animationFrames - 1; i++) {      
        animationSrcs[i] = (Rectangle{(float)(src.x), (float)(src.y + i * 16), (float)16, (float)16});           
    }
}

void PlayerAnimation::changeAnimation(PlayerState& state)
{
    if (state.curState == MOVING)
    {
        curRec = animationSrcs[curAnimation];  
        curRec.x += curFrames[curAnimation] * 16;   
    } 

    if (state.curState == IDLE)
    {
        curRec.x = 0;
    }
}



void PlayerAnimation::update(PlayerState& state)
{
    timeSinceLastFrameSwap += GetFrameTime();

    if (timeSinceLastFrameSwap > animationUpdateTime)  
    {
        curFrames[curAnimation] < 4 ? curFrames[curAnimation]++ : curFrames[curAnimation] = 0; // increment curFrame after certain amount of time
        timeSinceLastFrameSwap = 0.0f;
    }
    
    changeAnimation(state);
}

Player::Player(Vector2 spawnPos, Rectangle src, int animationFrames) 
    : animation_(src, animationFrames), physics_(spawnPos)
{
    
}


void Player::update(Atlas& atlas) 
{
    physics_.update();
    input_.update(physics_, animation_, state_);
    animation_.update(state_);



    DrawTextureRec(atlas.texture, animation_.curRec, physics_.pos, WHITE);
}