#include "atlas.hpp"
#include "macros_util.hpp"
#include "player.hpp"
#include "raylib.h"

class UI {
  public:
    Rectangle bounds = {0, (float)(GetScreenHeight() - 48), NUM_HOTBAR * 48, NUM_HOTBAR * 48};
    bool mouseOutOfBounds(); // check if mouse is not in the ui bound space
    const float gridSpacing = 2.0f;
    void hotBar(Atlas &atlas, PlayerInventory &inventory);
};
