#pragma once
#include "dev_util.hpp"
#include "entt/entity/registry.hpp"
#include "lua/lualoader.hpp"
#include "scene.hpp"
#include "tile_manager.hpp"
#include <unordered_map>

using PathMap = std::unordered_map<Vector2, Vector2, Vector2Util, Vector2Util>;

using PathVisited = std::unordered_map<Vector2, PathNode, Vector2Util, Vector2Util>;

using PathQueue = std::priority_queue<PathNode, std::vector<PathNode>, PathNodeComparison>;

struct BoolVec2Pair {
    bool confirm;
    Vector2 pos;
};

// controls npc pathing and updating tilemanager entity cache
class NPCSystem {
  public:
    TileManager* tManager;
    entt::basic_registry<>* sRegistry;
    entt::entity playerID;

    // entities that need cache updates
    std::queue<entt::entity> entities_marked_for_cache;

    bool NPC_DEBUG_INFO = LuaGetBool("DRAW_NPC_DEBUG", "scripts/game_settings.lua");

    // initialization
    NPCSystem(TileManager* tileManager, entt::basic_registry<>* EntityRegistry, entt::entity player);
    void addNPCs();

    // handle functions
    void update(Scene& scene);
    bool astar(entt::entity id);
    void moveNPC(entt::entity id); // move particular npc

    // querying cache
    bool entityAtPosition(Vector2 pos); // returns true if entity at position
    BoolVec2Pair searchItem(entt::entity id, int itemID, int radius);
    bool nearEntity(entt::entity id, int radius);

    // cache handling
    void cachePosition(Vector2 pos, entt::entity id);
    void clearCachePosition(Vector2 pos, entt::entity id);
    void clearCacheBefore();
    void updateCacheAfter();

    // debug
    bool showEntityInfo(Camera2D& camera);

  private:
    // astar helper function
    void reconstructPath(PathMap cameFrom, Vector2 current, entt::entity id);

    // actions
    void handleActions();
    void giveRandomPath(entt::entity id);
    void checkSearching(entt::entity id);
    void handleEating(entt::entity id);
    void handleLeisure(entt::entity id);
    void handleSocial(entt::entity id);

    void showDebugInfo(entt::entity id);
};
