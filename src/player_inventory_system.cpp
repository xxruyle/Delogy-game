#include "player_inventory_system.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "dev_util.hpp"
#include "raylib.h"
#include "lua/lualoader.hpp"
#include <iostream>

void PlayerInventorySystem::updateInventorySelection(InventoryC& inventory)
{
    int key = InputSystem::getUserKeypress();

    int NUM_HOTBAR = LuaGetInt("NUM_HOTBAR", "scripts/game_settings.lua");
    if (key > 0 && key < NUM_HOTBAR + 1) { // make sure keys are not negative
        inventory.curItem = key - 1;       // set cur hot bar item
    }
}

void PlayerInventorySystem::updateItemRotation(InventoryC& inventory)
{

    int rotationKey = InputSystem::getUserKeypress();

    if (rotationKey == ITEM_ROTATION) {
        int curItemID = inventory.hotbar[inventory.curItem];
        if (curItemID >= RAIL_NW && curItemID <= RAIL_SW) {
            if (curItemID == 6) {
                inventory.hotbar[inventory.curItem] = RAIL_NW; // the first RAIL item
            }
            else {
                inventory.hotbar[inventory.curItem]++; // go to next rail item if it is not the last
            }
        }
    }
}

void PlayerInventorySystem::drawCurItem(Atlas& atlas, Camera2D& camera, InventoryC& inventory)
{
    int itemID = inventory.hotbar[inventory.curItem];
    Vector2 atlasSrc = {(float)itemids[itemID].x, (float)itemids[itemID].y};
    Vector2 mousePosition = getMouseGridPosition(camera);
    mousePosition.x *= 16;
    mousePosition.y *= 16;

    DrawTextureRec(atlas.texture, {atlasSrc.x, atlasSrc.y, atlas.width, atlas.height}, mousePosition, Color{255, 255, 255, 170});
}

void PlayerInventorySystem::update(Scene& scene)
{
    InventoryC& inv = scene.EntityRegistry.get<InventoryC>(scene.player);

    updateInventorySelection(inv);
    updateItemRotation(inv);
}
