#include "components.hpp"
#include "macros_util.hpp"
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
#include "player_movement_system.hpp"
#include "player_inventory_system.hpp"
#include "npc_system.hpp"
#include "entity_data.hpp"
#include "entt/entity/registry.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);

    /* SetRandomSeed(30); */

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");

    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();

    UI userInterface;
    CartManager cartManager;

    SpriteDrawSystem drawSystem;
    AnimationSystem animationSystem;
    PlayerInventorySystem inventorySystem;

    Scene scene;
    scene.addPlayer(AtlasType::MEDIUM, {1 * 16, 1 * 16}, {4, 4, 32, 32}, 4, 4);

    NPCSystem npcSystem;

    PlayerMovementSystem playerMovementSystem;

    /* npcSystem.update(scene); */
    int count = 0;
    while (!WindowShouldClose()) {
        // count++;
        BeginDrawing();

        ClearBackground(BLACK);

        scene.setPlayerFocus();
        scene.updateCamera();

        BeginMode2D(scene.camera);

        /* Handle Tile Manager */
        tileManager.update(drawSystem.smallAtlas, userInterface, scene);
        /* Handle Carts */
        cartManager.update(tileManager, scene);

        /* Systems */
        npcSystem.moveNPCs(scene.EntityRegistry);
        inventorySystem.update(scene);
        animationSystem.update(scene.EntityRegistry, scene.player);

        /* Player Movement */
        playerMovementSystem.update(scene.player, scene.EntityRegistry, tileManager);
        /* Collisions */

        /* Draw */
        drawSystem.drawSprites(scene.EntityRegistry);

        inventorySystem.drawCurItem(drawSystem.smallAtlas, scene.camera,
                                    scene.EntityRegistry.get<InventoryC>(scene.player));

        drawMouseGridOutline(scene.camera, RED);
        EndMode2D();

        /* Draw UI */
        PhysicsC physicsComponent = scene.EntityRegistry.get<PhysicsC>(scene.player);
        drawGameInfo(scene.camera, scene.playerPosition, scene.EntityRegistry.get<PhysicsC>(scene.player).velocity);
        userInterface.hotBar(drawSystem.smallAtlas, scene.EntityRegistry.get<InventoryC>(scene.player));

        EndDrawing();
    }
    CloseWindow();
}
