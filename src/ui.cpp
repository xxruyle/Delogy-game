#include "ui.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "ui_util.hpp"

void UI::showHotBar(Atlas &atlas, PlayerInventory &inventory)
{

    UIRowGridRec({0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR, inventory.curHotbarItem,
                 RAYWHITE, Color{255, 255, 255, 32});

    UIRowGridIcon(atlas, inventory, {0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR);

    bounds = {0, (float)(GetScreenHeight() - 48), (NUM_HOTBAR * 48) + (gridSpacing * NUM_HOTBAR), NUM_HOTBAR * 48};
    /* DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, PURPLE); */
}
