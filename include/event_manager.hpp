#pragma once
#include "entt/entity/entity.hpp"
#include "raylib.h"
#include <deque>

enum EventType {
    ITEM_DELETION,
    ITEM_CREATION,
    ITEM_GET, // player gets item
};

struct ItemEvent {
    EventType type;
    int itemID;
    Vector2 itemPos;
    entt::entity relatedEntity; // entity causing the event
};

namespace EventManager {

extern std::deque<ItemEvent> itemEventQueue;
void addItemDeletionEvent(Vector2 gridPos, int itemID, entt::entity relatedEntity);
void addItemCreationEvent(Vector2 gridPos, int itemID, entt::entity relatedEntity);

} // namespace EventManager
