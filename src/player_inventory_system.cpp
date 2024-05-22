#include "player_inventory_system.hpp"
#include "dev_util.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <iostream>

void PlayerInventorySystem::updateInventorySelection(InventoryC& inventory)
{
    int key = InputSystem::getUserKeypress();

    if (key > 0 && key < inventory.NUM_HOTBAR + 1) { // make sure keys are not negative
        inventory.curItem = key - 1;                 // set cur hot bar item
    }
}

void PlayerInventorySystem::updateItemRotation(InventoryC& inventory)
{

    if (inventory.curItem >= 0 && inventory.curItem <= inventory.NUM_HOTBAR) {
        int rotationKey = InputSystem::getUserKeypress();
        if (rotationKey == ITEM_ROTATION) {
            int curItemID = inventory.slots[inventory.curItem];
            if (curItemID >= RAIL_NW && curItemID <= RAIL_SW) { // rotatable items
                if (curItemID == 6) {
                    inventory.slots[inventory.curItem] = RAIL_NW; // the first RAIL item
                }
                else {
                    inventory.slots[inventory.curItem]++; // go to next rail item if it is not the last
                }
            }
        }
    }
}

void PlayerInventorySystem::drawCurItem(Atlas& atlas, Camera2D& camera, InventoryC& inventory)
{
    int itemID = inventory.slots[inventory.curItem];
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
