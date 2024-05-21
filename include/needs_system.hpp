#pragma once
#include "components.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"

class NeedsSystem {
public:
  entt::basic_registry<> *sRegistry;
  NeedsSystem(entt::basic_registry<> *EntityRegistry);

  void setCurrentDesire(NeedsC &need, GenesC &genes);
  void handleCurrentAction(NeedsC &need, GenesC &genes);
  void updateDesires(NeedsC &need, GenesC &genes);
  void clampDesires(NeedsC &need, GenesC &genes);
  void update();
};
