#include "player.hpp"
#include "dev_util.hpp"
#include "macros_util.hpp"
#include "player_camera.hpp"
#include "raylib.h"
#include "raymath.h"
#include "input_util.hpp"

void PlayerPhysics::update()
{
    pos.x += velocity.x * GetFrameTime();
    pos.y += velocity.y * GetFrameTime();
}

void PlayerPhysics::clampSpeed()
{
    if (velocity.x > maxSpeed) {
        velocity.x = maxSpeed;
    }
    else if (velocity.x < -maxSpeed) {
        velocity.x = -maxSpeed;
    }

    if (velocity.y > maxSpeed) {
        velocity.y = maxSpeed;
    }
    else if (velocity.y < -maxSpeed) {
        velocity.y = -maxSpeed;
    }

    float velMag = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (velMag > maxSpeed) {
        float scale = maxSpeed / velMag;
        velocity.x *= scale;
        velocity.y *= scale;
    }
}

void PlayerInput::getInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state)
{
    if (IsKeyDown(KEY_A)) // left
    {
        state.curState = MOVING;
        physics.dir.x -= 1;
    }

    if (IsKeyDown(KEY_D)) // right
    {
        state.curState = MOVING;
        physics.dir.x += 1;
    }

    if (IsKeyDown(KEY_W)) // up
    {
        state.curState = MOVING;
        physics.dir.y -= 1;
    }

    if (IsKeyDown(KEY_S)) // down
    {
        state.curState = MOVING;
        physics.dir.y += 1;
    }

    physics.clampSpeed();
    physics.dir = Vector2Normalize(physics.dir); // fixes faster diagonal movement

    physics.velocity.x += physics.dir.x * physics.acceleration * GetFrameTime();
    physics.velocity.y += physics.dir.y * physics.acceleration * GetFrameTime();
}

void PlayerInput::getGamePadInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state)
{
    gamepadDir.x = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_LEFT_X);
    gamepadDir.y = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_LEFT_Y);

    // Check left joystick for movement
    if (gamepadDir.x < 0) // left
    {
        state.curState = MOVING;
        physics.dir.x -= 1;
    }

    if (gamepadDir.x > 0) // right
    {
        state.curState = MOVING;
        physics.dir.x += 1;
    }

    if (gamepadDir.y < 0) // up
    {
        state.curState = MOVING;
        physics.dir.y -= 1;
    }

    if (gamepadDir.y > 0) // down
    {
        state.curState = MOVING;
        physics.dir.y += 1;
    }

    physics.clampSpeed();
    physics.dir = Vector2Normalize(physics.dir);

    physics.velocity.x += gamepadDir.x * physics.acceleration * GetFrameTime();
    physics.velocity.y += gamepadDir.y * physics.acceleration * GetFrameTime();
}
void PlayerInput::getGamePadMouseInput()
{
    Vector2 mousePos = GetMousePosition();

    rightStickAxis.x = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_RIGHT_X);
    rightStickAxis.y = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_RIGHT_Y);

    mousePos.x += rightStickAxis.x * JOYSTICK_RIGHT_SENSITIVITY;
    mousePos.y += rightStickAxis.y * JOYSTICK_RIGHT_SENSITIVITY;

    SetMousePosition((int)mousePos.x, (int)mousePos.y);
}

void PlayerInput::resetGamePadInput(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state)
{
    if (gamepadDir.x >= 0 && physics.velocity.x < 0) {
        animation.curFrames[LEFT] = 0;

        physics.velocity.x += physics.decel * GetFrameTime();

        if (physics.velocity.x > 0) {
            physics.velocity.x = 0;
        }
    }

    if (gamepadDir.x <= 0 && physics.velocity.x > 0) {
        animation.curFrames[RIGHT] = 0;
        physics.velocity.x -= physics.decel * GetFrameTime();

        if (physics.velocity.x < 0) {
            physics.velocity.x = 0;
        }
    }

    if (gamepadDir.y >= 0 && physics.velocity.y < 0) {
        animation.curFrames[UP] = 0;
        physics.velocity.y += physics.decel * GetFrameTime();

        if (physics.velocity.y < 0) {
            physics.velocity.y = 0;
        }
    }

    if (gamepadDir.y <= 0 && physics.velocity.y > 0) {
        animation.curFrames[DOWN] = 0;

        physics.velocity.y -= physics.decel * GetFrameTime();

        if (physics.velocity.y > 0) {
            physics.velocity.y = 0;
        }
    }
}

bool PlayerInput::isGamepadBeingUsed(PlayerState &state)
{

    if (!IsGamepadAvailable(curGamepad)) {
        return false;
    }
    getGamePadMouseInput();

    gamepadDir.x = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_LEFT_X);
    gamepadDir.y = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_LEFT_Y);

    rightStickAxis.x = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_RIGHT_X);
    rightStickAxis.y = GetGamepadAxisMovement(curGamepad, GAMEPAD_AXIS_RIGHT_Y);

    bool gamepadBeingUsed;

    if (abs(gamepadDir.x) == 0 && abs(gamepadDir.y) == 0) {
        state.curState = IDLE;
        gamepadBeingUsed = false;
        ShowCursor();
    }
    else {
        HideCursor();
        gamepadBeingUsed = true;
    }

    return gamepadBeingUsed;
}

void PlayerInput::resetInput(PlayerAnimation &animation, PlayerState &state, PlayerPhysics &physics)
{
    if (!IsKeyDown(KEY_A) && physics.velocity.x < 0) // LEFT
    {
        animation.curFrames[LEFT] = 0;

        physics.dir.x = 0;
        physics.velocity.x += physics.decel * GetFrameTime();

        if (physics.velocity.x > 0) {
            physics.velocity.x = 0;
        }
    }

    if (!IsKeyDown(KEY_D) && physics.velocity.x > 0) // RIGHT
    {
        animation.curFrames[RIGHT] = 0;

        physics.dir.x = 0;

        physics.velocity.x -= physics.decel * GetFrameTime();

        if (physics.velocity.x < 0) {
            physics.velocity.x = 0;
        }
    }

    if (!IsKeyDown(KEY_W) && physics.velocity.y < 0) // up
    {
        animation.curFrames[UP] = 0;

        physics.dir.y = 0;

        physics.velocity.y += physics.decel * GetFrameTime();

        if (physics.velocity.x > 0) {
            physics.velocity.y = 0;
        }
    }

    if (!IsKeyDown(KEY_S) && physics.velocity.y > 0) // down
    {
        animation.curFrames[DOWN] = 0;

        physics.dir.y = 0;

        physics.velocity.y -= physics.decel * GetFrameTime();

        if (physics.velocity.y < 0) {
            physics.velocity.y = 0;
        }
    }

    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) {
        state.curState = IDLE;
    }
}

void PlayerInput::getGamePadAnimation(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state)
{
    Vector2 curDir;
    switch (animation.curAnimation) {
    case DOWN:
        curDir = {0, 1};
        break;
    case UP:
        curDir = {0, -1};
        break;
    case RIGHT:
        curDir = {1, 0};
    case LEFT:
        curDir = {-1, 0};
    default:
        break;
    }

    Vector2 nGamePadDir = Vector2Normalize(gamepadDir);
    Vector2 uDir = {0, -1};
    float dirDot = Vector2Angle(uDir, nGamePadDir) * 180.0f / PI;
    float dirToVelAngle = abs(Vector2DotProduct(curDir, nGamePadDir));
    if (dirToVelAngle < .54f || state.curState == IDLE) {
        if ((dirDot >= 157 && dirDot <= 180) || (dirDot >= 0 && dirDot <= 22)) // UP
        {
            animation.curAnimation = UP;
        }
        else if (dirDot >= 22 && dirDot < 67) { // RIGHT
            animation.curAnimation = RIGHT;
        }
        else if (dirDot >= 67 && dirDot <= 112) { // DOWN
            animation.curAnimation = DOWN;
        }
        else if (dirDot >= 112 && dirDot <= 157) { // LEFT
            animation.curAnimation = LEFT;
        }
    }
}

void PlayerInput::getAnimationInput(PlayerAnimation &animation)
{
    if (IsKeyDown(KEY_A) && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))) {
        animation.curAnimation = LEFT;
    }

    if (IsKeyDown(KEY_D) && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))) {
        animation.curAnimation = RIGHT;
    }

    if (IsKeyDown(KEY_W) && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S))) {
        animation.curAnimation = UP;
    }

    if (IsKeyDown(KEY_S) && !(IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W))) {
        animation.curAnimation = DOWN;
    }
}

void PlayerInput::getInteractState(PlayerCamera &camera, PlayerState &state, PlayerInventory &inventory)
{
    if (isDestroyButtonPressed(curGamepad) && !camera.freeCam) {
        state.curAction = DESTROY;
    }
    else if (isCreateButtonPressed(curGamepad) && !camera.freeCam) {
        if (inventory.itemHotbar[inventory.curHotbarItem] == CART) {
            state.curAction = ENTITY_CREATE;
        }
        else {
            state.curAction = ITEM_CREATE;
        }
    }
    else {
        state.curAction = NORMAL;
    }
}

void PlayerInput::getInventoryChoice(PlayerInventory &inventory)
{
    int key = GetKeyPressed();
    if (key != 0) {
        int num = key - 48;                     // converting from ascii decimal value to actual keyboard values
        if (num >= 0 && num < NUM_HOTBAR + 1) { // make sure keys are not negative
            inventory.curHotbarItem = num - 1;  // set cur hot bar item
        }
    }

    if (IsGamepadButtonPressed(curGamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
        if (inventory.curHotbarItem < NUM_HOTBAR - 1) {
            inventory.curHotbarItem += 1;
        }
        else {
            inventory.curHotbarItem = 0;
        }
    }
}

void PlayerInput::getItemRotation(PlayerInventory &inventory)
{
    // add more rotations for items
    int curItemID = inventory.itemHotbar[inventory.curHotbarItem];
    if (isRotationButtonPressed(curGamepad) && curItemID >= RAIL_NW && curItemID <= RAIL_SW) {
        if (curItemID == 6) {
            inventory.itemHotbar[inventory.curHotbarItem] = RAIL_NW; // the first RAIL item
        }
        else {
            inventory.itemHotbar[inventory.curHotbarItem]++; // go to next rail item if it is not the last
        }
    }
}

void PlayerInput::update(PlayerPhysics &physics, PlayerAnimation &animation, PlayerState &state, PlayerCamera &camera,
                         PlayerInventory &inventory)
{

    if (isGamepadBeingUsed(state)) {
        getGamePadAnimation(physics, animation, state);
        resetGamePadInput(physics, animation, state);
        getGamePadInput(physics, animation, state);
    }
    else {
        getAnimationInput(animation);
        resetInput(animation, state, physics);
        getInput(physics, animation, state);
    }

    // we want this to be the last call for state info passing to TileManager
    getInteractState(camera, state, inventory);
    getInventoryChoice(inventory);
    getItemRotation(inventory);
}

PlayerAnimation::PlayerAnimation(Rectangle src, int animationFrames) : frameAmount(animationFrames)
{
    for (int i = 0; i < animationFrames; i++) {
        animationSrcs[i] =
            (Rectangle{(float)(src.x), (float)(src.y + i * (16 + SPRITE_PADDING)), (float)16, (float)16});
    }

    curRec = animationSrcs[DOWN];
}

void PlayerAnimation::changeAnimation(PlayerState &state)
{
    int furthestX = animationSrcs[curAnimation].x +
                    ((16 + SPRITE_PADDING) * (frameAmount - 1)); // the furthestX a sprite can be in the atlas
    switch (state.curState) {
    case MOVING:
        curRec = animationSrcs[curAnimation];
        // check for integer errors
        if (curFrames[curAnimation] >= 0 && (curFrames[curAnimation] * 16 <= furthestX)) {
            curRec.x += curFrames[curAnimation] * (16 + SPRITE_PADDING); // go to the next frame
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
    drawItem(camera_.cam, atlas, inventory_.itemHotbar[inventory_.curHotbarItem]);

    animation_.update(state_);
    input_.update(physics_, animation_, state_, camera_, inventory_);
    physics_.update();
    camera_.update(physics_.pos);
}
