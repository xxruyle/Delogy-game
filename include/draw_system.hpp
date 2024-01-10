#pragma once
#include "components.hpp"
#include <entt/entity/registry.hpp>
#include <vector>

class SpriteDrawSystem {
  public:
    void drawSprites(Texture2D &atlas, entt::basic_registry<> &registry);
    void update(){};
};
