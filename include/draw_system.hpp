#pragma once
#include "atlas.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include <entt/entity/registry.hpp>
#include <vector>

class SpriteDrawSystem {
  public:
    Atlas smallAtlas;
    Atlas mediumAtlas;

    SpriteDrawSystem() : smallAtlas("res/real_atlas.png", 16, 16), mediumAtlas("res/medium_atlas.png", 32, 32){};

    void drawSprites(Atlas &atlas, entt::basic_registry<> &registry);
    void update(){};
};
