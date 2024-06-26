#pragma once
#include "atlas.hpp"
#include <entt/entity/registry.hpp>

class SpriteDrawSystem {
  public:
    Atlas smallAtlas;
    Atlas mediumAtlas;

    SpriteDrawSystem() : smallAtlas("res/real_atlas.png", 16, 16), mediumAtlas("res/medium_atlas.png", 32, 32){};

    void drawSprites();
    void update(){};
};
