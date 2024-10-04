#include "atlas_data.hpp"
#include "components.hpp"
#include "ecs_registry.hpp"
#include "history.hpp"
#include "item_manager.hpp"
#include "macros_util.hpp"
#include <iostream>
#include <utility>
#define RAYGUI_IMPLEMENTATION
#include "animation_system.hpp"
#include "cart_manager.hpp"
#include "dev_util.hpp"
#include "draw_system.hpp"
#include "event_manager.hpp"
#include "input_system.hpp"
#include "inventory_system.hpp"
#include "minimap.hpp"
#include "movement_system.hpp"
#include "needs_system.hpp"
#include "npc_system.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "tile_manager.hpp"
#include "wireframe.hpp"

#include "entity_data.hpp"
#include "entt/entity/registry.hpp"

#include "cache_manager.hpp"
#include "lua/lualoader.hpp"

int main()
{
	SetTraceLogLevel(LOG_ERROR); // removes debug info in console
	SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	// hotloaded lua stuff -----------------------------------------------
	Slua::init({"scripts/game_settings.lua", "scripts/atlas_data.lua"});
	AtlasData::init();
	// -------------------------------------------------------------------

	/*SetTargetFPS(60);*/
	InitWindow(Slua::lua["WINDOW_WIDTH"].get_or(0), Slua::lua["WINDOW_HEIGHT"].get_or(0), "Delogy Indev");
	int randomSeed = Slua::lua["WORLD_SEED"].get_or(0);
	SetRandomSeed(randomSeed);

	TileManager tileManager(randomSeed);

	MiniMap miniMap(Slua::lua["MINIMAP_WIDTH"].get_or(0), Slua::lua["MINIMAP_HEIGHT"].get_or(0), &tileManager);

	CartManager cartManager;

	SpriteDrawSystem drawSystem;
	AnimationSystem animationSystem;
	UI userInterface(&drawSystem);
	InventorySystem inventorySystem(&userInterface);
	Scene scene;

	scene.addPlayer(AtlasType::MEDIUM, {1 * 16, 1 * 16}, {4, 4, 32, 32}, 4, 4);

	NPCSystem npcSystem(&tileManager, scene.player);

	NeedsSystem needsSystem;

	MovementSystem movementSystem;

	tileManager.generateChunks();

	/* npcSystem.update(scene); */
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		scene.updateCamera(userInterface);
		BeginMode2D(scene.camera);
		{
			/* Handle Tile Manager */
			tileManager.update(drawSystem.smallAtlas, userInterface, scene);

			/* Handle Carts */
			cartManager.update(tileManager, scene);

			/* Systems */
			needsSystem.update();
			npcSystem.update(scene);

			npcSystem.clearCacheBefore(); // clear cache before movement starts
			movementSystem.update(scene.player, tileManager);
			npcSystem.updateCacheAfter(); // update cache after movement is finished

			animationSystem.update(scene.player);

			/* Draw */
			drawSystem.drawSprites();
			inventorySystem.drawCurItem(drawSystem.smallAtlas, scene.camera, ECS::registry.get<InventoryC>(scene.player), ECS::registry.get<HotBarC>(scene.player));
			WireFrame::draw();
			drawMouseGridOutline(scene.camera, Color{255, 255, 255, 180});
			npcSystem.showEntityInfo(scene.camera);
		}
		EndMode2D();

		/* Handle Input*/
		inventorySystem.update(scene);

		/* Draw UI */
		PhysicsC physicsComponent = ECS::registry.get<PhysicsC>(scene.player);
		drawGameInfo(scene.camera, scene.playerPosition, ECS::registry.get<PhysicsC>(scene.player).velocity);

		miniMap.update(userInterface, Vector2{scene.playerPosition.x + 6, scene.playerPosition.y + 16}, drawSystem);

		ItemManager::update(scene);
		History::update();

		EndDrawing();
	}
	CloseWindow();
}
