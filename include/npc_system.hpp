#pragma once
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "lua/lualoader.hpp"
#include "scene.hpp"
#include "tile_manager.hpp"
#include <unordered_map>

using PathMap = std::unordered_map<Vector2, Vector2, Vector2Util, Vector2Util>;

using PathVisited =
    std::unordered_map<Vector2, PathNode, Vector2Util, Vector2Util>;

using PathQueue =
    std::priority_queue<PathNode, std::vector<PathNode>, PathNodeComparison>;

struct BoolVec2Pair {
  bool confirm;
  Vector2 pos;
};

// controls npc pathing and updating tilemanager entity cache
class NPCSystem {
public:
  TileManager *tManager;
  entt::basic_registry<> *sRegistry;
  entt::entity playerID;

  std::queue<entt::entity> entities_marked_for_cache;

  bool NPC_DEBUG_INFO =
      LuaGetBool("DRAW_NPC_DEBUG", "scripts/game_settings.lua");

  NPCSystem(TileManager *tileManager, entt::basic_registry<> *EntityRegistry,
            entt::entity player);

  bool showEntityInfo(Camera2D &camera);
  void update(Scene &scene);
  void updateNPCPaths();
  void addNPCs();
  bool astar(entt::entity id);
  void moveNPC(entt::entity id); // move particular npc

  void cachePosition(Vector2 pos, entt::entity id);
  void clearCachePosition(Vector2 pos, entt::entity id);
  void clearCacheBefore();
  void updateCacheAfter();

  bool entityAtPosition(
      Vector2 pos); // returns an entity at position if it exists, otherwise 0

  BoolVec2Pair searchItem(entt::entity id, int itemID);
  bool nearEntity(entt::entity id);

  bool npcAtItem(entt::entity id, int itemID);

private:
  void reconstructPath(PathMap cameFrom, Vector2 current, entt::entity id);

  void randomPath(entt::entity id);
  void showDebugInfo(entt::entity id);
  void handleEating(entt::entity id);
  void handleLeisure(entt::entity id);
  void handleSocial(entt::entity id);
};
