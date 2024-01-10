#pragma once
#include "components.hpp"
#include "entt/entity/registry.hpp"
#include "scene.hpp"

class NPCSystem {
  public:
    void addNPCs(entt::basic_registry<> &registry);
    void moveNPCs(entt::basic_registry<> &registry);
    void update(Scene &scene);
};
