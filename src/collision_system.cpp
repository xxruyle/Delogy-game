#include "collision_system.hpp"
#include "components.hpp"
#include "input_system.hpp"
#include "macros_util.hpp"
#include "raylib.h"

void CollisionSystem::update(entt::basic_registry<> &registry)
{
    if (InputSystem::getUserKeypress() == DEBUG_WIREFRAME) {
        showWireFrame ? showWireFrame = false : showWireFrame = true;
    }

    auto view = registry.view<PositionC, PhysicsC, CollisionC>();

    for (auto entity : view) {
        auto &position = view.get<PositionC>(entity);
        auto &physics = view.get<PhysicsC>(entity);
        auto &collision = view.get<CollisionC>(entity);

        if (showWireFrame) {
            DrawRectangleLinesEx(Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y,
                                           collision.aabb.width, collision.aabb.height},
                                 0.2f, WHITE);
        }
    }
}
