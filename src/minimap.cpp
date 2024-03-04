#include "minimap.hpp"
#include "raylib.h"
#include "ui_util.hpp"
#include "macros_util.hpp"
#include "input_system.hpp"

void obtainArea(TileManager &tileManager) {}

void MiniMap::draw()
{
    float padding = 3.0f;

    if (InputSystem::getUserKeypress() == OPEN_MINIMAP) {
        if (fullScreen)
            fullScreen = false;
        else
            fullScreen = true;
    }

    if (!fullScreen) {
        Rectangle tempPosition = Rectangle{GetScreenWidth() - width - padding, padding, width, height};
        UIWindowOutline(tempPosition);

        position = tempPosition;
    }
    else {
        Rectangle tempPosition = Rectangle{0, 0, GetScreenWidth(), GetScreenHeight()};
        DrawText("full minimap mode!", GetScreenWidth() - 300, 0, 20, WHITE);
        position = tempPosition;
    }
}

void MiniMap::update()
{
    // obtainArea(Vector2 playerPos, TileManager &tileManager);
    draw();
}
