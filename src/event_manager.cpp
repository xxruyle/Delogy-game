#include "event_manager.hpp"
#include "item_manager.hpp"
#include <deque>
#include <iostream>

namespace EventManager {

std::deque<ItemEvent> itemEventQueue;

void addItemDeletionEvent(Vector2 gridPos, int itemID, entt::entity relatedEntity)
{
	ItemEvent e = {ITEM_DELETION, itemID, gridPos, relatedEntity};
	ItemManager::deleteItem(gridPos);
	EventManager::itemEventQueue.push_back(e);
}

void addItemCreationEvent(Vector2 gridPos, int itemID, entt::entity relatedEntity)
{
	ItemEvent e = {ITEM_CREATION, itemID, gridPos, relatedEntity};
	ItemManager::placeItem(gridPos, (ItemType)e.itemID);
	EventManager::itemEventQueue.push_back(e);
}

} // namespace EventManager
