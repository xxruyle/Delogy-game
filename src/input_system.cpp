#include "input_system.hpp"
#include "raylib.h"
#include <iostream>

int InputSystem::getUserKeypress()
{
    int key = GetKeyPressed();
    if (key) {
        return key - 48; // converting from ascii decimal value to actual keyboard values
    }

    if (IsKeyPressed(Keys[ITEM_ROTATION])) {
        return ITEM_ROTATION;
    }

    return NULL_PRESS;
}

int InputSystem::getMovementDirection()
{

    if (IsKeyDown(Keys[MOVE_RIGHT]) &&
        !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_UP]) || IsKeyDown(Keys[MOVE_DOWN]))) {
        return MOVE_RIGHT;
    }

    if (IsKeyDown(Keys[MOVE_LEFT]) &&
        !(IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_UP]) || IsKeyDown(Keys[MOVE_DOWN]))) {
        return MOVE_LEFT;
    }

    if (IsKeyDown(Keys[MOVE_UP]) &&
        !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_DOWN]))) {
        return MOVE_UP;
    }

    if (IsKeyDown(Keys[MOVE_DOWN]) &&
        !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_UP]))) {
        return MOVE_DOWN;
    }

    return NULL_PRESS;
}
Vector2 InputSystem::getDirectionVector()
{
    Vector2 direction = {0, 0};

    if (IsKeyDown(Keys[MOVE_LEFT])) {
        direction.x -= 1;
    }

    if (IsKeyDown(Keys[MOVE_RIGHT])) {
        direction.x += 1;
    }

    if (IsKeyDown(Keys[MOVE_UP])) {
        direction.y -= 1;
    }

    if (IsKeyDown(Keys[MOVE_DOWN])) {
        direction.y += 1;
    }

    return Vector2Normalize(direction);
}

float InputSystem::getScrollAmount() { return GetMouseWheelMove(); }

int InputSystem::getUserMouseInteraction()
{
    if (IsMouseButtonPressed(Keys[PLAYER_DESTROY])) {
        return PLAYER_DESTROY;
    }

    if (IsMouseButtonPressed(Keys[PLAYER_CREATE])) {
        return PLAYER_CREATE;
    }

    return NULL_PRESS;
}
