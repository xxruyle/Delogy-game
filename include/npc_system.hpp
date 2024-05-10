#pragma once
#include "components.hpp"
#include "dev_util.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "scene.hpp"
#include "tile_manager.hpp"

class NPCSystem {
public:
  TileManager *tManager;
  entt::basic_registry<> *sRegistry;
  NPCSystem(TileManager *tileManager, entt::basic_registry<> *EntityRegistry);
  void addNPCs();

  void moveNPC(entt::entity id); // move particular npc
  void moveNPCs();
  void update(Scene &scene);
  void cachePosition(Vector2 pos, entt::entity id);
  void clearCachePosition(Vector2 pos, entt::entity id);

  bool showEntityInfo(Camera2D &camera);
};
