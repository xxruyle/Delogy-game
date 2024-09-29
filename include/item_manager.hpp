#pragma once
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "lua/lualoader.hpp"
#include "raylib.h"
#include "scene.hpp"

// manages normal items and special items (items that go in cache and have
// components)
namespace ItemManager {
extern bool survival_mode;
void placeItem(Vector2 pos, ItemType itemID);
void deleteItem(Vector2 pos);
void interactWithItem(entt::entity id);
void checkPlayerDeletions(Scene& scene);
void checkPlayerCreations(Scene& scene);
void checkPlayerInteractions(Scene& scene);
void checkItemGets();
void addItemToInventory(int itemID, entt::entity id);
void update(Scene& scene);

} // namespace ItemManager
