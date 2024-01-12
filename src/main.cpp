#include "macros_util.hpp"
#include <utility>
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
#include "collision_system.hpp"
#include "entt/entity/registry.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");
    /* SetTargetFPS(60); */

    Atlas atlas("res/real_atlas.png", 16, 16);

    SetRandomSeed(30);
    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();

    UI userInterface;
    CartManager cartManager;

    SpriteDrawSystem smallDrawSystem;
    AnimationSystem animationSystem;
    PlayerInventorySystem inventorySystem;
    CollisionSystem collisionSystem;

    Scene scene;
    scene.addPlayer(AtlasType::MEDIUM, {0, 0}, {4, 4, 32, 32}, 4, 4);

    NPCSystem npcSystem;

    InputSystem input;
    PlayerMovementSystem playerMovementSystem;

    /* npcSystem.update(scene); */
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        scene.setPlayerFocus();
        scene.updateCamera();

        BeginMode2D(scene.camera);

        /* Handle Tile Manager */
        tileManager.update(atlas, input, userInterface, scene);
        /* Handle Carts */
        cartManager.update(tileManager, input, scene);

        /* Systems */
        npcSystem.moveNPCs(scene.EntityRegistry);
        inventorySystem.update(scene, input);
        animationSystem.update(input, scene.EntityRegistry, scene.player);

        smallDrawSystem.drawSprites(atlas, scene.EntityRegistry);

        collisionSystem.update(scene.EntityRegistry);

        playerMovementSystem.update(input, scene.player, scene.EntityRegistry);

        inventorySystem.drawCurItem(atlas, scene.camera, scene.EntityRegistry.get<InventoryC>(scene.player));
        drawMouseGridOutline(scene.camera, WHITE);
        EndMode2D();

        /* Draw UI */
        drawGameInfo(scene.camera, scene.playerPosition);
        userInterface.hotBar(atlas, scene.EntityRegistry.get<InventoryC>(scene.player));

        EndDrawing();
    }
    CloseWindow();
}
