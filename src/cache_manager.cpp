#include "cache_manager.hpp"

namespace CacheManager {
CacheMap entityCache;
CacheMap itemCache;

bool entityAtPosition(Vector2 pos)
{
	if (entityCache.count(pos)) {
		return entityCache[pos].size() > 0;
	}

	return false;
}

void cacheEntity(Vector2 pos, entt::entity id) { entityCache[pos].push_back(id); }

void clearCachePosition(Vector2 pos, entt::entity id)
{
	// loop over every  entity at position
	for (int i = 0; i < entityCache[pos].size(); i++) {
		entt::entity entID = entityCache[pos][i];

		// if the entity id is what we are looking for erase it
		if (entID == id) {
			std::vector<entt::entity>::iterator it = entityCache[pos].begin() + i;
			entityCache[pos].erase(it);
		}
	}

	// deleting the position key if no entity exists in it
	if (!entityCache.count(pos)) {
		for (auto it = entityCache.begin(); it != entityCache.end();) {
			if (it->first.x == pos.x && it->first.y == pos.y) {
				it = entityCache.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

} // namespace CacheManager
