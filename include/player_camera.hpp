#pragma once
#include "raylib.h"

class PlayerCamera {
  public:
    Camera2D cam = {0};
    Vector2 playerPos;
    float windowWidth;
    float windowHeight;
    bool freeCam = false;

    PlayerCamera();

    void update(Vector2 target);
    void updateWindowSize();
    void updateTarget(Vector2 target);
    void updateZoom();
    void updateFreeCam();
};
