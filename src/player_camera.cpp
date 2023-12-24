#include "player_camera.hpp"
#include "dev_util.hpp"
#include "macros_util.hpp"
#include "raymath.h"

PlayerCamera::PlayerCamera()
{
    cam.zoom = 1.9f;
    cam.offset = Vector2{(float)((float)WINDOW_WIDTH / 2), (float)((float)WINDOW_HEIGHT / 2)};
    windowWidth = WINDOW_WIDTH;
    windowHeight = WINDOW_HEIGHT;
}

void PlayerCamera::update(Vector2 target)
{
    if (IsKeyPressed(KEY_Y)) // enable free cam
    {
        if (freeCam) {
            freeCam = false;
            updateTarget(target);
        }
        else {
            freeCam = true;
        }
    }

    if (!freeCam) {
        updateTarget(target);
        cam.offset = Vector2{(float)((float)windowWidth / 2), (float)((float)windowHeight / 2)};
    }
    else {
        drawMouseGridOutline(cam, RAYWHITE);
        updateFreeCam();
    }

    updateZoom();
    updateWindowSize();
}

void PlayerCamera::updateFreeCam()
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = Vector2Scale(GetMouseDelta(), -1.0f / cam.zoom);

        cam.target = Vector2Add(cam.target, delta);
    }

    if (GetMouseWheelMove() != 0) {
        // get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cam);

        // set the offset to where the mouse is
        cam.offset = GetMousePosition();

        // set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        cam.target = mouseWorldPos;
    }
}

void PlayerCamera::updateTarget(Vector2 target) { cam.target = Vector2Add(target, Vector2{8.0f, 8.0f}); }

void PlayerCamera::updateZoom()
{
    cam.zoom += GetMouseWheelMove() * 0.25f;

    if (cam.zoom > 7.0f)
        cam.zoom = 7.0f;

    if (cam.zoom < .062f)
        cam.zoom = .062f;
}

void PlayerCamera::updateWindowSize()
{
    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();
}
