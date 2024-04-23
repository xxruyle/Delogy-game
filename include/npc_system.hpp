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
  NPCSystem(TileManager *tileManager);
  void addNPCs(entt::basic_registry<> &registry);
  void moveNPCs(entt::basic_registry<> &registry);
  void update(Scene &scene);
  void cachePosition(Vector2 pos, entt::entity id);
};
