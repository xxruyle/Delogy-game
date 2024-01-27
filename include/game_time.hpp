#pragma once
#include "raylib.h"

class GameTime {
public:
  static float getDT() {

    float dt = GetFrameTime();

    if (dt > 0.1f) {
      dt = 0.0f;
    }

    return dt;
  }
};
