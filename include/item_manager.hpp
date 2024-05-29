#pragma once
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"

// manages the amount of items there is
namespace ItemManager {

void addItem(Vector2 pos, int itemID, int amount);
void decrementItem(Vector2 pos, int amount);

} // namespace ItemManager
