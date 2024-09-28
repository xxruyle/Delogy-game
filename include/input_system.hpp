#pragma once
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include <deque>
#include <entt/entity/registry.hpp>

#define NUM_KEYS 15
class InputSystem {
public:
  static int getUserKeypress();
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
