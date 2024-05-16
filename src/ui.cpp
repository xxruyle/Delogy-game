#include "ui.hpp"
#include "raylib.h"
#include "macros_util.hpp"
#include "ui_util.hpp"
#include "lua/lualoader.hpp"

void UI::hotBar(Atlas& atlas, InventoryC& inventory)
{
    int NUM_HOTBAR = LuaGetInt("NUM_HOTBAR", "scripts/game_settings.lua");

    UIRowGridRec({0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR, inventory.curItem, RAYWHITE, Color{255, 255, 255, 32});

    UIRowGridIcon(atlas, inventory, {0, (float)(GetScreenHeight() - 48), 48, 48}, 1.0f, gridSpacing, NUM_HOTBAR);
    UIRowGridNumbers({5, (float)(GetScreenHeight() - 65), 48, 48}, gridSpacing, NUM_HOTBAR);

    Rectangle bound = {0, (float)(GetScreenHeight() - 48), (NUM_HOTBAR * 48) + (gridSpacing * NUM_HOTBAR), NUM_HOTBAR * 48};
    bounds[HOTBAR] = bound;
    /* DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, PURPLE); */
}

void UI::miniMap(Rectangle mapSrc)
{
    UIWindowOutline(mapSrc);
    /* UIWindowOutline(Rectangle{mapSrc.x + 40, mapSrc.y + mapSrc.height + 2, mapSrc.width - 80, 10}); // extra */
    bounds[MINIMAP] = mapSrc;
}

bool UI::mouseOutOfBounds()
{
    for (int i = 0; i < 2; i++) {
        if (CheckCollisionPointRec(GetMousePosition(), bounds[i])) {
            return false;
        }
    }
    return true;
}
