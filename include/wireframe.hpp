#pragma once
#include "components.hpp"
#include "input_system.hpp"
#include "raylib.h"

class WireFrame {
  public:
    static bool wireframe_on;
    static void draw();
};
