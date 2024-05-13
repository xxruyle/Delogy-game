#pragma once
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "scene.hpp"
#include "tile_manager.hpp"
#include <unordered_map>

using PathMap = std::unordered_map<Vector2, Vector2, Vector2Util, Vector2Util>;

// controls npc pathing and updating tilemanager entity cache
class NPCSystem {
public:
  TileManager *tManager;
  entt::basic_registry<> *sRegistry;
  entt::entity playerID;
  NPCSystem(TileManager *tileManager, entt::basic_registry<> *EntityRegistry,
            entt::entity player);
  void addNPCs();

  bool astar(entt::entity id);
  void reconstructPath(PathMap cameFrom, Vector2 current, entt::entity id);

  void moveNPC(entt::entity id); // move particular npc
  void moveNPCs();
  void update(Scene &scene);
  void cachePosition(Vector2 pos, entt::entity id);
  void clearCachePosition(Vector2 pos, entt::entity id);

  bool showEntityInfo(Camera2D &camera);
};
