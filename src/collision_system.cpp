#include "collision_system.hpp"
#include "components.hpp"
#include "macros_util.hpp"
#include "raylib.h"

void CollisionSystem::update(entt::basic_registry<> &registry)
{
    if (IsKeyPressed(KEY_C)) {
        showWireFrame ? showWireFrame = false : showWireFrame = true;
    }

    auto view = registry.view<PositionC, PhysicsC>();

    for (auto entity : view) {
        auto &position = view.get<PositionC>(entity);
        auto &physics = view.get<PhysicsC>(entity);

        if (showWireFrame) {
            DrawRectangleLinesEx(Rectangle{position.pos.x + physics.aabb.x, position.pos.y + physics.aabb.y,
                                           physics.aabb.width, physics.aabb.height},
                                 0.2f, WHITE);
        }
    }
}
