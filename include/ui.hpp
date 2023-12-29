#include "atlas.hpp"
#include "macros_util.hpp"
#include "player.hpp"
#include "raylib.h"

class UI {
  public:
    Rectangle bounds = {0, (float)(GetScreenHeight() - 48), NUM_HOTBAR * 48, NUM_HOTBAR * 48};
    float gridSpacing = 2.0f;
    void showHotBar(Atlas &atlas, PlayerInventory &inventory);
};
