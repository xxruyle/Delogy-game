#pragma once
#include "components.hpp"
#include "entt/entity/registry.hpp"
#include "input_system.hpp"
#include "raylib.h"

class WireFrame {
public:
  static bool wireframe_on;
  static void draw(entt::basic_registry<> &sceneRegistry);
};
