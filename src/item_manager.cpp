#include "item_manager.hpp"
#include "cache_manager.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"

namespace ItemManager {
void addItem(Vector2 pos, int itemID, int amount)
{
	entt::entity entity = ECS::registry.create();

	ECS::registry.emplace<PositionC>(entity, pos);
	ECS::registry.emplace<ItemC>(entity, itemID, amount);
	CacheManager::cacheEntity(pos, entity);
}

void decrementItem(Vector2 pos, int amount)
{
	if (CacheManager::entityAtPosition(pos)) {
		entt::entity id = CacheManager::getItemAtPosition(pos);

		ItemC& item = ECS::registry.get<ItemC>(id);

		item.capacity -= 1;

		if (item.capacity <= 0) {
			CacheManager::clearCachePosition(pos, id);
		}
	}
}

} // namespace ItemManager
