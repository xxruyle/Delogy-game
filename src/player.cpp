#include "player.hpp"
#include "dev_util.hpp"
#include "player_camera.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

void PlayerPhysics::update()
{
    clampSpeed();

    // std::cout << velocity.x << ' ' << velocity.y << std::endl;

    pos.x += velocity.x * GetFrameTime();
    pos.y += velocity.y * GetFrameTime();
}

void PlayerPhysics::clampSpeed()
{
    float velMag = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (velMag > maxSpeed) {
        float scale = maxSpeed / velMag;
        velocity.x *= scale;
        velocity.y *= scale;

        velocity.x = floor(velocity.x);
        velocity.y = floor(velocity.y);
    }
}

void PlayerInput::getInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state)
{
    if (IsKeyDown(KEY_A)) // left
    {
        state.curState = MOVING;
        animation.curAnimation = LEFT;
        physics.dir.x -= 1;
    }

    if (IsKeyDown(KEY_D)) // right
    {
        state.curState = MOVING;
        animation.curAnimation = RIGHT;
        physics.dir.x += 1;
    }

    if (IsKeyDown(KEY_W)) // up
    {
        state.curState = MOVING;
        animation.curAnimation = UP;
        physics.dir.y -= 1;
    }

    if (IsKeyDown(KEY_S)) // down
    {
        state.curState = MOVING;
        animation.curAnimation = DOWN;
        physics.dir.y += 1;
    }

    physics.dir = Vector2Normalize(physics.dir);

    physics.velocity.x += physics.dir.x * physics.acceleration;
    physics.velocity.y += physics.dir.y * physics.acceleration;
}

void PlayerInput::resetInput(PlayerAnimation &animation, PlayerState &state, PlayerPhysics &physics)
{
    if (!IsKeyDown(KEY_A) && physics.velocity.x < 0) // LEFT
    {

        animation.curFrames[LEFT] = 0;

        physics.dir.x = 0;
        physics.velocity.x += physics.decel;

        if (physics.velocity.x > 0) {
            physics.velocity.x = 0;
        }
    }

    if (!IsKeyDown(KEY_D) && physics.velocity.x > 0) // RIGHT
    {
        animation.curFrames[RIGHT] = 0;

        physics.dir.x = 0;

        physics.velocity.x -= physics.decel;

        if (physics.velocity.x < 0) {
            physics.velocity.x = 0;
        }
    }

    if (!IsKeyDown(KEY_W) && physics.velocity.y < 0) // up
    {
        animation.curFrames[UP] = 0;

        physics.dir.y = 0;

        physics.velocity.y += physics.decel;

        if (physics.velocity.x > 0) {
            physics.velocity.y = 0;
        }
    }

    if (!IsKeyDown(KEY_S) && physics.velocity.y > 0) // down
    {
        animation.curFrames[DOWN] = 0;

        physics.dir.y = 0;

        physics.velocity.y -= physics.decel;

        if (physics.velocity.y < 0) {
            physics.velocity.y = 0;
        }
    }

    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) {
        state.curState = IDLE;
    }
}

void PlayerInput::getInteractState(PlayerCamera &camera, PlayerState &state)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !camera.freeCam) {
        state.curAction = DESTROY;
        state.curState = INTERACTING;
    }
    else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !camera.freeCam) {
        state.curAction = CREATE;
        state.curState = INTERACTING;
    }
    else {
        state.curAction = NORMAL;
    }
}

void PlayerInput::update(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state, PlayerCamera &camera)
{
    resetInput(animation, state, physics);
    getInput(physics, animation, state);
    // we want this to be the last call for state info passing to TileManager
    getInteractState(camera, state);
}

PlayerAnimation::PlayerAnimation(Rectangle src, int animationFrames) : frameAmount(animationFrames)
{
    for (int i = 0; i < animationFrames; i++) {
        animationSrcs[i] = (Rectangle{(float)(src.x), (float)(src.y + i * 16), (float)16, (float)16});
    }

    curRec = animationSrcs[DOWN];
}

void PlayerAnimation::changeAnimation(PlayerState &state)
{
    int furthestX =
        animationSrcs[curAnimation].x + (16 * (frameAmount - 1)); // the furthestX a sprite can be in the atlas
    switch (state.curState) {
    case MOVING:
        curRec = animationSrcs[curAnimation];
        // check for integer errors
        if (curFrames[curAnimation] >= 0 && (curFrames[curAnimation] * 16 <= furthestX)) {
            curRec.x += curFrames[curAnimation] * 16; // go to the next frame
        }
        break;
    case IDLE:
        curFrames[curAnimation] = 0;
        curRec = animationSrcs[curAnimation];
        break;
    default:
        break;
    };
}

void PlayerAnimation::update(PlayerState &state)
{
    timeSinceLastFrameSwap += GetFrameTime();

    if (timeSinceLastFrameSwap > animationUpdateTime) {
        curFrames[curAnimation] < frameAmount - 1
            ? curFrames[curAnimation]++
            : curFrames[curAnimation] = 0; // increment curFrame after certain amount of time
        timeSinceLastFrameSwap = 0.0f;
    }

    changeAnimation(state);
}

Player::Player(Vector2 spawnPos, Rectangle src, int animationFrames)
    : animation_(src, animationFrames), physics_(spawnPos)
{
}

void Player::update(Atlas &atlas)
{

    DrawTextureRec(atlas.texture, animation_.curRec, physics_.pos, WHITE);

    animation_.update(state_);
    input_.update(physics_, animation_, state_, camera_);
    physics_.update();
    camera_.update(physics_.pos);
}
