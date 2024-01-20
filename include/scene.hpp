#pragma once
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"
#include <entt/entity/registry.hpp>

class SceneCamera {
  public:
    Camera2D camera = {Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, {0.0f, 0.0f}, 0.0f, 2.0f};
    void updateCamera();
    void setPlayerFocus(Vector2 playerPos);
    void setFreeCamFocus(Vector2 pos);

  private:
};

class Scene {
  public:
    entt::basic_registry<> EntityRegistry;
    Camera2D camera = {Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, {0.0f, 0.0f}, 0.0f, 2.0f};
    entt::entity player;
    Vector2 playerPosition = {0, 0};
    Vector2 curTarget = {0, 0};
    Vector2 prevCameraTarget = {0, 0};

    void updateCamera();
    void setPlayerFocus();
    void addPlayer(AtlasType atlasid, Vector2 spawnPos, Rectangle frameSrc, int numFrames, int framesPerRow);

  private:
    int windowWidth;
    int windowHeight;
};
