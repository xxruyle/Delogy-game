#pragma once
#include "input_system.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include "ui.hpp"
#include <entt/entity/registry.hpp>

class Scene {
public:
  entt::basic_registry<> EntityRegistry;
  float targetZoom = 0.0f;
  bool isZooming = false;
  Camera2D camera = {
      Vector2{LuaGetInt("WINDOW_WIDTH", "scripts/game_settings.lua") / 2,
              LuaGetInt("WINDOW_HEIGHT", "scripts/game_settings.lua") / 2},
      {0.0f, 0.0f},
      0.0f,
      2.0f};
  entt::entity player;
  Vector2 playerPosition = {0, 0};
  Vector2 curTarget = {0, 0};
  Vector2 prevCameraTarget = {0, 0};

  void updateCamera(UI &ui);
  void setPlayerFocus();
  void addPlayer(AtlasType atlasid, Vector2 spawnPos, Rectangle frameSrc,
                 int numFrames, int framesPerRow);

private:
  int windowWidth;
  int windowHeight;
};
