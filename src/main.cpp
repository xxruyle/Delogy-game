#include "macros_util.hpp"
#define RAYGUI_IMPLEMENTATION
#include "dev_util.hpp"
#include "raylib.h"
#include "tile_manager.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Delogy Indev");
    SetTargetFPS(60);

    Atlas atlas("res/default_atlas.png");

    Player player(Vector2{0, 0}, Rectangle{80, 0}, 4);

    UI userInterface;
    TileManager tileManager(GetRandomValue(0, 3000));
    tileManager.generateChunks();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        BeginMode2D(player.camera_.cam);

        /* Handle Tile Manager */
        tileManager.update(atlas, player, userInterface);

        /* Draw Entities */
        player.update(atlas);
        EndMode2D();

        /* Draw UI */
        drawGameInfo(player);

        userInterface.hotBar(atlas, player.inventory_);
        EndDrawing();
    }
    CloseWindow();
}
