#pragma once

#include <math.h>
#include "raylib.h"

void drawMouseGridOutline(Camera2D& camera, Color color); // draw the outline of a grid depending on the coordinate the mouse is hovering over 
Vector2 getMouseGridPosition(Camera2D& camera);