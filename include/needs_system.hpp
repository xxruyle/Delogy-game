#pragma once
#include "components.hpp"

class NeedsSystem {
  public:
    void setCurrentDesire(NeedsC& need, GenesC& genes);
    void handleCurrentAction(NeedsC& need, GenesC& genes);
    void updateDesires(NeedsC& need, GenesC& genes);
    void clampDesires(NeedsC& need, GenesC& genes);
    void update();
};
