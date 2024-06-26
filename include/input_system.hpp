#pragma once
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include <deque>
#include <entt/entity/registry.hpp>

#define NUM_KEYS 15
enum KeyBindings {
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

static int Keys[NUM_KEYS] = {KEY_NULL,
                             KEY_D,
                             KEY_A,
                             KEY_W,
                             KEY_S,
                             MOUSE_BUTTON_LEFT,
                             MOUSE_BUTTON_RIGHT,
                             KEY_I,
                             KEY_E,
                             KEY_R,
                             KEY_M,
                             KEY_LEFT_SHIFT,
                             MOUSE_BUTTON_LEFT,
                             MOUSE_BUTTON_LEFT,
                             KEY_C};

class InputSystem {
public:
  static int getUserKeypress();
  static int getUserCharpress();
  static int getUserKeydown();
  static int getUserMouseInteraction();
  static int getMovementDirection();
  static Vector2 getDirectionVector(); // get a sum of all movement vectors for
                                       // each cardinal direction
  static float getScrollAmount();

  static bool getMoveLeft();
  static bool getMoveRight();
  static bool getMoveUp();
  static bool getMoveDown();

  static void
  changeKeyBinding(int keybindID,
                   int newKey); // change an existing keybindID to newKey
};
