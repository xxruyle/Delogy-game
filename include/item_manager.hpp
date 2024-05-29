#pragma once
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"

// manages the amount of items there is 
namespace ItemManager 
{
    void addItem(Vector2 worldPos, int itemID, int amount, entt::basic_registry<>* sRegistry);
    void deleteItem(Vector2 worldPos, entt::basic_registry<>* sRegistry);

}
