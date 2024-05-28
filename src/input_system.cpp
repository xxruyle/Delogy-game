#include "input_system.hpp"
#include "dev_util.hpp"
#include "raylib.h"
#include <deque>

int InputSystem::getUserKeypress()
{
	int key = GetKeyPressed();
	if (key) {
		return key - 48; // converting from ascii decimal value to actual keyboard values
	}

	if (IsKeyPressed(Keys[ITEM_ROTATION])) {
		return ITEM_ROTATION;
	}

	if (IsKeyPressed(Keys[DEBUG_WIREFRAME])) {
		return DEBUG_WIREFRAME;
	}

	if (IsKeyPressed(Keys[OPEN_MINIMAP])) {
		return OPEN_MINIMAP;
	}

	if (IsKeyPressed(Keys[OPEN_INVENTORY])) {
		return OPEN_INVENTORY;
	}

	return NULL_PRESS;
}

int InputSystem::getUserKeydown()
{
	if (IsKeyDown(Keys[SHOW_INFO])) {
		return SHOW_INFO;
	}

	return NULL_PRESS;
}

int InputSystem::getUserCharpress()
{
	int key = GetCharPressed();

	if (key) {
		return key - 48;
	}
}

bool InputSystem::getMoveRight() { return IsKeyDown(Keys[MOVE_RIGHT]); }
bool InputSystem::getMoveLeft() { return IsKeyDown(Keys[MOVE_LEFT]); }
bool InputSystem::getMoveUp() { return IsKeyDown(Keys[MOVE_UP]); }
bool InputSystem::getMoveDown() { return IsKeyDown(Keys[MOVE_DOWN]); }

int InputSystem::getMovementDirection()
{

	if (IsKeyDown(Keys[MOVE_RIGHT]) && !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_UP]) || IsKeyDown(Keys[MOVE_DOWN]))) {
		return MOVE_RIGHT;
	}

	if (IsKeyDown(Keys[MOVE_LEFT]) && !(IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_UP]) || IsKeyDown(Keys[MOVE_DOWN]))) {
		return MOVE_LEFT;
	}

	if (IsKeyDown(Keys[MOVE_UP]) && !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_DOWN]))) {
		return MOVE_UP;
	}

	if (IsKeyDown(Keys[MOVE_DOWN]) && !(IsKeyDown(Keys[MOVE_LEFT]) || IsKeyDown(Keys[MOVE_RIGHT]) || IsKeyDown(Keys[MOVE_UP]))) {
		return MOVE_DOWN;
	}

	return NULL_PRESS;
}
Vector2 InputSystem::getDirectionVector()
{
	Vector2 direction = {0, 0};

	if (IsKeyDown(Keys[MOVE_LEFT])) {
		direction.x -= 1;
	}

	if (IsKeyDown(Keys[MOVE_RIGHT])) {
		direction.x += 1;
	}

	if (IsKeyDown(Keys[MOVE_UP])) {
		direction.y -= 1;
	}

	if (IsKeyDown(Keys[MOVE_DOWN])) {
		direction.y += 1;
	}

	return Vector2Normalize(direction);
}

float InputSystem::getScrollAmount() { return GetMouseWheelMove(); }

int InputSystem::getUserMouseInteraction()
{

	if (IsMouseButtonPressed(Keys[PLAYER_DESTROY])) {
		return PLAYER_DESTROY;
	}

	if (IsMouseButtonPressed(Keys[PLAYER_CREATE])) {
		return PLAYER_CREATE;
	}

	if (IsMouseButtonDown(Keys[CLICK_HOLD])) {
		return CLICK_HOLD;
	}

	if (IsMouseButtonReleased(Keys[MOVE_RIGHT])) {
		return MOVE_RIGHT;
	}

	return NULL_PRESS;
}
void InputSystem::changeKeyBinding(int keybindID, int newKey) { Keys[keybindID] = newKey; }
