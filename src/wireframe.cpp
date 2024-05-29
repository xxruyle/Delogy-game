#include "wireframe.hpp"
#include "ecs_registry.hpp"
#include "input_system.hpp"

bool WireFrame::wireframe_on = false;
void WireFrame::draw()
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
		auto view = ECS::registry.view<PositionC, CollisionC>();

		for (auto entity : view) {

			PositionC& position = view.get<PositionC>(entity);
			CollisionC& collision = view.get<CollisionC>(entity);

			DrawRectangleLinesEx(Rectangle{position.pos.x + collision.aabb.x, position.pos.y + collision.aabb.y, collision.aabb.width, collision.aabb.height}, 0.2f, WHITE);
		}
	}
}
