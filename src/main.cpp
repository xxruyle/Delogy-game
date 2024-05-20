#include "components.hpp"
#include "macros_util.hpp"
#include "tile_data.hpp"
#include <utility>
#include <iostream>
#define RAYGUI_IMPLEMENTATION
#include "dev_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include "cart_manager.hpp"
#include "scene.hpp"
#include "draw_system.hpp"
#include "input_system.hpp"
#include "animation_system.hpp"
#include "movement_system.hpp"
#include "player_inventory_system.hpp"
#include "npc_system.hpp"
#include "minimap.hpp"
#include "wireframe.hpp"
#include "needs_system.hpp"

#include "entity_data.hpp"
#include "entt/entity/registry.hpp"

#include "lua/lualoader.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitLua(); // intialize lua before calling lualoader functions

    /* SetTargetFPS(60); */

    InitWindow(LuaGetInt("WINDOW_WIDTH", "scripts/game_settings.lua"), LuaGetInt("WINDOW_HEIGHT", "scripts/game_settings.lua"), "Delogy Indev");
    SetRandomSeed(LuaGetInt("WORLD_SEED", "scripts/game_settings.lua"));

    TileManager tileManager(LuaGetInt("WORLD_SEED", "scripts/game_settings.lua"));

    MiniMap miniMap(LuaGetInt("MINIMAP_WIDTH", "scripts/game_settings.lua"), LuaGetInt("MINIMAP_HEIGHT", "scripts/game_settings.lua"), &tileManager);
    UI userInterface;

    CartManager cartManager;

    SpriteDrawSystem drawSystem;
    AnimationSystem animationSystem;
    PlayerInventorySystem inventorySystem;

    Scene scene;
    scene.addPlayer(AtlasType::MEDIUM, {1 * 16, 1 * 16}, {4, 4, 32, 32}, 4, 4);

    NPCSystem npcSystem(&tileManager, &scene.EntityRegistry, scene.player);
    npcSystem.addNPCs();

    NeedsSystem needsSystem(&scene.EntityRegistry);

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
            inventorySystem.update(scene);

            npcSystem.clearCacheBefore(); // clear cache before movement starts
            movementSystem.update(scene.player, scene.EntityRegistry, tileManager);
            npcSystem.updateCacheAfter(); // update cache after movement is finished

            npcSystem.update(scene);
            needsSystem.update();
            animationSystem.update(scene.EntityRegistry, scene.player);

            /* Draw */
            drawSystem.drawSprites(scene.EntityRegistry);

            inventorySystem.drawCurItem(drawSystem.smallAtlas, scene.camera, scene.EntityRegistry.get<InventoryC>(scene.player));

            WireFrame::draw(scene.EntityRegistry);

            drawMouseGridOutline(scene.camera, Color{255, 255, 255, 180});
            npcSystem.showEntityInfo(scene.camera);
            /* tileManager.clearEntityPositionCache(); */
        }
        EndMode2D();

        /* Draw UI */
        PhysicsC physicsComponent = scene.EntityRegistry.get<PhysicsC>(scene.player);
        drawGameInfo(scene.camera, scene.playerPosition, scene.EntityRegistry.get<PhysicsC>(scene.player).velocity);
        drawCounter("Cart Count: ", cartManager.cartCount);
        userInterface.hotBar(drawSystem.smallAtlas, scene.EntityRegistry.get<InventoryC>(scene.player));

        miniMap.update(userInterface, Vector2{scene.playerPosition.x + 6, scene.playerPosition.y + 16}, drawSystem);

        EndDrawing();
    }
    CloseWindow();

    CloseLua();
}
