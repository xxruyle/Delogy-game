#include "macros_util.hpp"
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
#include "entt/entity/registry.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");
    SetExitKey(KEY_ESCAPE);
    /* SetTargetFPS(60); */

    Atlas atlas("res/real_atlas.png");

    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();

    UI userInterface;

    CartManager cartManager;

    SpriteDrawSystem drawSystem;
    AnimationSystem animationSystem;
    PlayerInventorySystem inventorySystem;

    Scene scene;
    scene.addPlayer({0, 0}, {4, 4, 16, 16}, 4);

    InputSystem input;
    PlayerMovementSystem playerMovementSystem;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        scene.setPlayerFocus();
        scene.updateCamera();

        BeginMode2D(scene.camera);

        /* Handle Tile Manager */
        tileManager.update(atlas, input, userInterface, scene);
        /* Handle Carts */
        cartManager.update(atlas, tileManager);

        /* Systems */
        inventorySystem.update(scene, input);
        animationSystem.update(input, scene.EntityRegistry, scene.player);

        drawSystem.drawSprites(atlas.texture, scene.EntityRegistry);
        playerMovementSystem.update(input, scene.player, scene.EntityRegistry);

        inventorySystem.drawCurItem(scene.camera, atlas.texture, scene.EntityRegistry.get<InventoryC>(scene.player));

        drawMouseGridOutline(scene.camera, WHITE);
        EndMode2D();

        /* Draw UI */
        drawGameInfo(scene.camera, scene.playerPosition);
        userInterface.hotBar(atlas, scene.EntityRegistry.get<InventoryC>(scene.player));

        EndDrawing();
    }
    CloseWindow();
}
