#include "inventory_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "ecs_registry.hpp"
#include "input_system.hpp"
#include "item_data.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <iostream>

InventorySystem::InventorySystem(UI* userInterface) { ui = userInterface; }

void InventorySystem::updateInventorySelection(InventoryC& inventory, HotBarC& hotBar)
{
	int key = InputSystem::getUserCharpress();

	if (key != NULL_PRESS) {
		if (key > 0 && key <= hotBar.capacity && key < inventory.slots.size()) { // make sure keys are not negative
			hotBar.curItem = key - 1;											 // set cur hot bar item
		}
	}
}

void InventorySystem::updateItemRotation(InventoryC& inventory, HotBarC& hotBar)
{
	if (hotBar.curItem >= 0 && hotBar.curItem <= hotBar.capacity) {
		int rotationKey = InputSystem::getUserKeypress();
		if (rotationKey == ITEM_ROTATION) {
			int curItemID = inventory.slots[hotBar.curItem];
			if (curItemID >= RAIL_NW && curItemID <= RAIL_SW) { // rotatable items
				if (curItemID == 6) {
					inventory.slots[hotBar.curItem] = RAIL_NW; // the first RAIL item
				}
				else {
					inventory.slots[hotBar.curItem]++; // go to next rail item if it is not the last
				}
			}
		}
	}
}

void InventorySystem::drawCurItem(Atlas& atlas, Camera2D& camera, InventoryC& inventory, HotBarC& hotBar)
{
	int itemID = inventory.slots[hotBar.curItem];
	Vector2 atlasSrc = {(float)itemids[itemID].x, (float)itemids[itemID].y};
	Vector2 mousePosition = getMouseGridPosition(camera);
	mousePosition.x *= 16;
	mousePosition.y *= 16;

	DrawTextureRec(atlas.texture, {atlasSrc.x, atlasSrc.y, atlas.width, atlas.height}, mousePosition, Color{255, 255, 255, 170});
}

void InventorySystem::swapItem(InventoryC& inventory, int index1, int index2)
{
	int tempSlot = inventory.slots[index2];
	int tempStack = inventory.stacks[index2];

	inventory.slots[index2] = inventory.slots[index1];
	inventory.stacks[index2] = inventory.stacks[index1];

	inventory.slots[index1] = tempSlot;
	inventory.stacks[index1] = tempStack;
}

void InventorySystem::update(Scene& scene)
{
	auto view = ECS::registry.view<InventoryC>();

	for (auto& entity : view) {

		if (entity == scene.player) {
			updatePlayerInventory(scene);
		}
		else {
			auto& inv = view.get<InventoryC>(entity);
			UIInventoryC& invUI = ECS::registry.get<UIInventoryC>(entity);
			if (invUI.active) {
				ui->inventory(inv, invUI.srcPos, 50, 50, 5);
			}
		}
	}
}

void InventorySystem::updatePlayerInventory(Scene& scene)
{
	InventoryC& inv = ECS::registry.get<InventoryC>(scene.player);
	HotBarC& hotBar = ECS::registry.get<HotBarC>(scene.player);
	UIInventoryC& invUI = ECS::registry.get<UIInventoryC>(scene.player);

	// player hide UI
	if (InputSystem::getUserKeypress() == OPEN_INVENTORY) {
		invUI.active ? invUI.active = false : invUI.active = true;
	}

	if (invUI.active) {
		ui->inventory(inv, invUI.srcPos, 50, 50, 7);
	}
	else {
		updateItemRotation(inv, hotBar);
		updateInventorySelection(inv, hotBar);
		HotBarC& hBar = ECS::registry.get<HotBarC>(scene.player);
		ui->hotBar(inv, hBar, invUI.srcPos, 50, 50);
	}
}
