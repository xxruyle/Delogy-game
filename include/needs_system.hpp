#pragma once
#include "components.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"

struct HumanStates {
  void findFood(NeedsC &need, GenesC &genes);
  void findFriends(NeedsC &need, GenesC &genes);
  void rest(NeedsC &need, GenesC &genes);
};

class NeedsSystem {
public:
  entt::basic_registry<> *sRegistry;
  NeedsSystem(entt::basic_registry<> *EntityRegistry);
  HumanStates npcHuman;

  void setCurrentDesire(NeedsC &need, GenesC &genes);
  void resetDesireStates(NeedsC &need);
  void updateDesires(NeedsC &need, GenesC &genes);
  void clampDesires(NeedsC &need, GenesC &genes);
  void update();
};
