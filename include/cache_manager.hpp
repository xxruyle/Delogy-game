#pragma once
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "unordered_map"
#include <vector>

using CacheMap = std::unordered_map<Vector2, std::vector<entt::entity>,
                                    Vector2Util, Vector2Util>;

namespace CacheManager {
extern CacheMap entityCache;
bool entityAtPosition(Vector2 pos);
bool needsAtPosition(Vector2 pos);
entt::entity
getItemAtPosition(Vector2 pos); // returns the first item found at position

void cacheEntity(Vector2 pos, entt::entity id);
void clearCachePosition(Vector2 pos, entt::entity id);
} // namespace CacheManager
