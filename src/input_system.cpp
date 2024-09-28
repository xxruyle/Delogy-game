#include "input_system.hpp"
#include "dev_util.hpp"
#include "raylib.h"
#include "keybindings.hpp"
#include <deque>


int InputSystem::getUserKeypress()
{
	int key = GetKeyPressed();
	if (key) {
		return key - 48; // converting from ascii decimal value to actual keyboard values
	}

	return NULL_PRESS;
}



bool InputSystem::getMoveRight() { return IsKeyDown(Keybindings::binds[MOVE_RIGHT]); }
bool InputSystem::getMoveLeft() { return IsKeyDown(Keybindings::binds[MOVE_LEFT]); }
bool InputSystem::getMoveUp() { return IsKeyDown(Keybindings::binds[MOVE_UP]); }
bool InputSystem::getMoveDown() { return IsKeyDown(Keybindings::binds[MOVE_DOWN]); }

int InputSystem::getMovementDirection()
{

	if (IsKeyDown(Keybindings::binds[MOVE_RIGHT]) && !(IsKeyDown(Keybindings::binds[MOVE_LEFT]) || IsKeyDown(Keybindings::binds[MOVE_UP]) || IsKeyDown(Keybindings::binds[MOVE_DOWN]))) {
		return MOVE_RIGHT;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_LEFT]) && !(IsKeyDown(Keybindings::binds[MOVE_RIGHT]) || IsKeyDown(Keybindings::binds[MOVE_UP]) || IsKeyDown(Keybindings::binds[MOVE_DOWN]))) {
		return MOVE_LEFT;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_UP]) && !(IsKeyDown(Keybindings::binds[MOVE_LEFT]) || IsKeyDown(Keybindings::binds[MOVE_RIGHT]) || IsKeyDown(Keybindings::binds[MOVE_DOWN]))) {
		return MOVE_UP;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_DOWN]) && !(IsKeyDown(Keybindings::binds[MOVE_LEFT]) || IsKeyDown(Keybindings::binds[MOVE_RIGHT]) || IsKeyDown(Keybindings::binds[MOVE_UP]))) {
		return MOVE_DOWN;
	}

	return NULL_PRESS;
}

Vector2 InputSystem::getDirectionVector()
{
	Vector2 direction = {0, 0};

	if (IsKeyDown(Keybindings::binds[MOVE_LEFT])) {
		direction.x -= 1;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_RIGHT])) {
		direction.x += 1;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_UP])) {
		direction.y -= 1;
	}

	if (IsKeyDown(Keybindings::binds[MOVE_DOWN])) {
		direction.y += 1;
	}

	return Vector2Normalize(direction);
}

float InputSystem::getScrollAmount() { return GetMouseWheelMove(); }

int InputSystem::getUserMouseInteraction()
{

	if (IsMouseButtonPressed(Keybindings::binds[PLAYER_DESTROY])) {
		return PLAYER_DESTROY;
	}

	if (IsMouseButtonPressed(Keybindings::binds[PLAYER_CREATE])) {
		return PLAYER_CREATE;
	}

	if (IsMouseButtonDown(Keybindings::binds[CLICK_HOLD])) {
		return CLICK_HOLD;
	}

	if (IsMouseButtonReleased(Keybindings::binds[MOVE_RIGHT])) {
		return MOVE_RIGHT;
	}

	return NULL_PRESS;
}
void InputSystem::changeKeyBinding(int keybindID, int newKey) { Keybindings::binds[keybindID] = newKey; }
