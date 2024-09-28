#pragma once
#include "macros_util.hpp"
#include "raylib.h"
#define NUM_KEYS 15

enum BindT {
    NULL_PRESS,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    PLAYER_DESTROY,
    PLAYER_CREATE,
    OPEN_INVENTORY,
    PLAYER_INTERACT,
    ITEM_ROTATION,
    OPEN_MINIMAP,
    SHOW_INFO,
    CLICK_HOLD,
    CLICK_RELEASE,
    DEBUG_WIREFRAME
};

namespace Keybindings {

    extern int binds[NUM_KEYS];
    void rebind(BindT old_key, int new_bind);
}