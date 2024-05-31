#include "scene.hpp"
#include "animation_system.hpp"
#include "components.hpp"
#include "dev_util.hpp"
#include "ecs_registry.hpp"
#include "lua/lualoader.hpp"
#include "macros_util.hpp"
#include "raylib.h"
#include "raymath.h"

void Scene::setPlayerFocus()
{

	PositionC& p1 = ECS::registry.get<PositionC>(player);
	playerPosition = p1.pos;
	curTarget = Vector2Add(playerPosition, Vector2{16.0f, 16.0f});
}

void Scene::updateCamera(UI& ui)
{

	setPlayerFocus();
	camera.target = curTarget;

	// update camera zoom
	if (ui.mouseOutOfBounds()) {
		// Calculate the target zoom level based on mouse wheel input
		float newZoom = camera.zoom + GetMouseWheelMove() * 0.4f; // Adjust sensitivity if needed
		if (abs(camera.zoom - newZoom) > 0.0f) {
			targetZoom = newZoom;
			isZooming = true;
		}

		if (isZooming) {
			camera.zoom = Lerp(camera.zoom, targetZoom, 0.04f);
			if (abs(camera.zoom - targetZoom) < 0.001f) {
				isZooming = false;
			}

			// zoom in: 10.0f
			// zoom out limit: 0.05f
			// debug mode
			camera.zoom = Clamp(camera.zoom, 0.05f, 10.0f);

			// actual game mode
			/* camera.zoom = Clamp(camera.zoom, 1.3f, 6.0f); */
		}
	}

	windowWidth = GetScreenWidth();
	windowHeight = GetScreenHeight();
	camera.offset = Vector2{round((float)(windowWidth / 2)), round((float)(windowHeight / 2))};
}

void Scene::addPlayer(AtlasType atlasid, Vector2 spawnPos, Rectangle frameSrc, int numFrames, int framesPerRow)
{
	entt::entity entity = ECS::registry.create();
	player = entity;

	ECS::registry.emplace<PlayerC>(entity, 0);
	ECS::registry.emplace<SpriteC>(entity, atlasid, frameSrc);
	ECS::registry.emplace<PositionC>(entity, spawnPos);
	ECS::registry.emplace<AnimationC>(entity, frameSrc, (unsigned int)numFrames, (unsigned int)framesPerRow);

	int playerSpeed = Slua::lua["PLAYER_SPEED"].get_or(0);
	ECS::registry.emplace<PhysicsC>(entity, Vector2{0.0f, 0.0f}, playerSpeed, true);

	float ATLAS_SPRITE_PADDING = Slua::lua["ATLAS_SPRITE_PADDING"].get_or(0.0f);
	float ATLAS_SPRITE_MARGIN = Slua::lua["ATLAS_SPRITE_MARGIN"].get_or(0.0f);
	ECS::registry.emplace<CollisionC>(entity, Rectangle{15 - ATLAS_SPRITE_MARGIN, 29 - ATLAS_SPRITE_MARGIN, 11, 7});

	ECS::registry.emplace<InventoryC>(entity, 20);
	ECS::registry.emplace<HotBarC>(entity, 5, 0);
	ECS::registry.emplace<UIInventoryC>(entity, false, Vector2{0, GetScreenHeight() - 51});
}
