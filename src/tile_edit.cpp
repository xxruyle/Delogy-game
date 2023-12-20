#include "tile_edit.h"

void drawMouseGridOutline(Camera2D& camera, Color color)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);


    // help with negative coordinate accuracy 
    if (mousePos.x < 0) { 
        mousePos.x = (int)floor(mousePos.x / 16) * 16 ;
    } else { 
        mousePos.x = (int)(mousePos.x / 16) * 16;
    }

    if (mousePos.y < 0) { 
        mousePos.y = (int)floor(mousePos.y / 16) * 16;
    } else { 
        mousePos.y = (int)(mousePos.y / 16) * 16;
    }

    DrawRectangleLines(mousePos.x, mousePos.y, 16, 16, color);
    

}

Vector2 getMouseGridPosition(Camera2D& camera)
{
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    // help with negative coordinate accuracy 
    if (mousePos.x < 0) { 
        mousePos.x = (int)floor(mousePos.x / 16);
    } else { 
        mousePos.x = (int)(mousePos.x / 16);
    }

    if (mousePos.y < 0) { 
        mousePos.y = (int)floor(mousePos.y / 16);
    } else { 
        mousePos.y = (int)(mousePos.y / 16);
    }

    return mousePos;
}