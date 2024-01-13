#include "ui.hpp"

#include "raylib.h"
#include "macros_util.hpp"

void UI::hotBar(Atlas &atlas, InventoryC &inventory)
{

    UIRowGridRec({0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR, inventory.curItem,
                 RAYWHITE, Color{255, 255, 255, 32});

    UIRowGridIcon(atlas, inventory, {0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR);
    UIRowGridNumbers({5, (float)(GetScreenHeight() - 65), 48, 48}, gridSpacing, NUM_HOTBAR);

    bounds = {0, (float)(GetScreenHeight() - 48), (NUM_HOTBAR * 48) + (gridSpacing * NUM_HOTBAR), NUM_HOTBAR * 48};
    /* DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, PURPLE); */
}

bool UI::mouseOutOfBounds() { return !CheckCollisionPointRec(GetMousePosition(), bounds); }
