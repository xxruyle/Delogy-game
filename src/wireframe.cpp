#include "wireframe.hpp"
#include "input_system.hpp"

bool WireFrame::wireframe_on = false;
void WireFrame::draw(entt::basic_registry<> &sceneRegistry)
{

    if (InputSystem::getUserKeypress() == DEBUG_WIREFRAME) {
        if (wireframe_on) {
            wireframe_on = false;
        }
        else {
            wireframe_on = true;
        }
    }

    if (wireframe_on) {
        auto view = sceneRegistry.view<PositionC, CollisionC>();

        for (auto entity : view) {

            PositionC &position = view.get<PositionC>(entity);
            CollisionC &collision = view.get<CollisionC>(entity);

            DrawRectangleLinesEx(Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y,
                                           collision.aabb.width, collision.aabb.height},
                                 0.2f, WHITE);
        }
    }
}
