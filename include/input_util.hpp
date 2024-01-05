#include "raylib.h"
#include "macros_util.hpp"
#define JOYSTICK_RIGHT_SENSITIVITY 1.8f

bool isDestroyButtonPressed(int gamepad) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ;
}

bool isCreateButtonPressed(int gamepad) {
    return IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ||
             IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
}


bool isRotationButtonPressed(int gamepad) {
    return IsKeyPressed(KEY_R) || IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
}


