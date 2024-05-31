#pragma once
#include "input_system.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include "ui.hpp" // TODO Not nearly useful enough to have this garbage spaghetti dependency with inv system

class Scene {
public:
  float targetZoom = 0.0f;
  bool isZooming = false;
  Camera2D camera = {Vector2{Slua::lua["WINDOW_WIDTH"].get_or(0) / 2,
                             Slua::lua["WINDOW_HEIGHT"].get_or(0) / 2},
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
