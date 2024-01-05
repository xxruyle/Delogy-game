#include "macros_util.hpp"
#define RAYGUI_IMPLEMENTATION
#include "dev_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"
#include "entity_manager.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");
    SetExitKey(KEY_ESCAPE);
    /* SetTargetFPS(60); */

    Atlas atlas("res/real_atlas.png");

    Player player(Vector2{0, 0}, Rectangle{4, 4}, 4);

    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();
    UI userInterface;

    EntityManager entityManager;
    /* entityManager.populateCarts(); */

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        BeginMode2D(player.camera_.cam);

        /* Handle Tile Manager */
        tileManager.update(atlas, player, userInterface);

        /* Handle Entities */
        entityManager.update(atlas, tileManager, player);

        /* Draw Entities */
        player.update(atlas);
        drawMouseGridOutline(player.camera_.cam, WHITE);

        EndMode2D();

        /* Draw UI */
        drawGameInfo(player);
        showEntityInfo(entityManager);
        userInterface.hotBar(atlas, player.inventory_);

        EndDrawing();
    }
    CloseWindow();
}
