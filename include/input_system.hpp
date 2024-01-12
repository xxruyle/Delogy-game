#pragma once
#include "raylib.h"
#include "raymath.h"
#include <entt/entity/registry.hpp>

#define NUM_KEYS 8
enum KeyBindings {
    NULL_PRESS,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    PLAYER_DESTROY,
    PLAYER_CREATE,
    ITEM_ROTATION
};

class InputSystem {
  public:
    int Keys[NUM_KEYS] = {KEY_NULL, KEY_D, KEY_A, KEY_W, KEY_S, MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, KEY_R};
    int getUserKeypress();
    int getUserMouseInteraction();
    int getMovementDirection();
    Vector2 getDirectionVector(); // get a sum of all movement vectors for each cardinal direction
    float getScrollAmount();

    bool getMoveLeft();
    bool getMoveRight();
    bool getMoveUp();
    bool getMoveDown();
};
