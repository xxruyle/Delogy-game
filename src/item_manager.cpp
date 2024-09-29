#include "item_manager.hpp"
#include "atlas_data.hpp"
#include "cache_manager.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "ecs_registry.hpp"
#include "event_manager.hpp"
#include "input_system.hpp"
#include "keybindings.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include <cstddef>
#include <utility>

namespace ItemManager {
bool survival_mode = Slua::lua.get<bool>("SURVIVAL_MODE");

void placeItem(Vector2 pos, ItemType itemID)
{
	switch (itemID) {
	case STORAGE_BOX: {
		entt::entity entity = ECS::registry.create();
		ECS::registry.emplace<InventoryC>(entity, 10);
		ECS::registry.emplace<UIInventoryC>(entity, false, Vector2{500, 500});
		ECS::registry.emplace<ItemC>(entity, STORAGE_BOX, 1);
		ECS::registry.emplace<PositionC>(entity, getGridToScreenPos(pos));
		CacheManager::cacheEntity(pos, entity);
		break;
	}
	default:
		break;
	}
}

void deleteItem(Vector2 pos)
{
	entt::entity id = CacheManager::getItemAtPosition(pos);

	if (id != entt::null) {
		if (ECS::registry.any_of<ItemC>(id)) {
			ItemC& item = ECS::registry.get<ItemC>(id);
			CacheManager::clearCachePosition(pos, id);
			ECS::registry.destroy(id);
			/*switch (item.id) {*/
			/*case STORAGE_BOX: {*/
			/*	break;*/
			/*}*/
			/*default: {*/
			/*	break;*/
			/*}*/
			/*}*/
		}
	}
}

void interactWithItem(entt::entity id)
{
	/* reset other inv uis activity to false so the one interacted
	 with is the only one active */
	auto view = ECS::registry.view<UIInventoryC, ItemC>();
	for (auto& other : view) {
		if (other != id) {
			UIInventoryC& otherUI = view.get<UIInventoryC>(other);
			otherUI.active = false;
		}
	}

	// if the item isn't null toggle its active
	if (id != entt::null) {
		if (ECS::registry.any_of<UIInventoryC>(id)) {
			auto& invUI = ECS::registry.get<UIInventoryC>(id);
			invUI.active ? invUI.active = false : invUI.active = true;
		}
	}
}

void checkItemGets()
{
	if (!EventManager::itemEventQueue.empty()) {
		ItemEvent e = EventManager::itemEventQueue.front();
		if (e.type == ITEM_GET) {
			std::cout << "ID: " << std::to_string((int)e.relatedEntity) << "; GET: " << e.itemID << std::endl;
			addItemToInventory(e.itemID, e.relatedEntity);
			EventManager::itemEventQueue.pop_front();
		}
	}
}

void addItemToInventory(int itemID, entt::entity id)
{
	InventoryC& inventory = ECS::registry.get<InventoryC>(id);
	for (int i = 0; i < inventory.slots.size(); i++) {
		if (inventory.slots[i] == NULL_ITEM) {
			inventory.slots[i] = itemID;
			inventory.stacks[i] += 1;
			// std::cout << "null: " << inventory.stacks[i] << std::endl;
			break;
		}
		else if (inventory.slots[i] == itemID) {
			inventory.stacks[i] += 1;
			// std::cout << "existing: " << inventory.stacks[i] << std::endl;
			break;
		}
	}
}

void checkPlayerDeletions(Scene& scene)
{
	if (IsMouseButtonPressed(Keybindings::binds[PLAYER_DESTROY])) {
		EventManager::addItemDeletionEvent(getMouseGridPosition(scene.camera), NULL_ITEM, scene.player);
	}
}

void checkPlayerCreations(Scene& scene)
{

	if (IsMouseButtonPressed(Keybindings::binds[PLAYER_CREATE])) {
		HotBarC& hotBar = ECS::registry.get<HotBarC>(scene.player);
		InventoryC& inventory = ECS::registry.get<InventoryC>(scene.player);
		if (inventory.stacks[hotBar.curItem] > 0 && inventory.stacks[hotBar.curItem] != NULL_ITEM) {
			EventManager::addItemCreationEvent(getMouseGridPosition(scene.camera), inventory.slots[hotBar.curItem], scene.player);
			if (survival_mode) {
				inventory.stacks[hotBar.curItem] -= 1;
			}
		}

		if (inventory.stacks[hotBar.curItem] <= 0) {
			inventory.slots[hotBar.curItem] = NULL_ITEM;
			inventory.stacks[hotBar.curItem] = 0;
		}
	}
}

void checkPlayerInteractions(Scene& scene)
{
	if (IsKeyPressed(Keybindings::binds[PLAYER_INTERACT])) {
		Vector2 mouseGridPos = getMouseGridPosition(scene.camera);
		entt::entity id = CacheManager::getItemAtPosition(mouseGridPos);
		interactWithItem(id);
	}
}

void update(Scene& scene)
{
	checkPlayerDeletions(scene);
	checkPlayerCreations(scene);
	checkPlayerInteractions(scene);
	checkItemGets();
}

} // namespace ItemManager
