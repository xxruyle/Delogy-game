#include "dev_util.h"

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

Vector2 getGridPosition(Vector2 screenWorldSpace)
{
     // help with negative coordinate accuracy 
    if (screenWorldSpace.x < 0) { 
        screenWorldSpace.x = (int)floor(screenWorldSpace.x / 16);
    } else { 
        screenWorldSpace.x = (int)(screenWorldSpace.x / 16);
    }

    if (screenWorldSpace.y < 0) { 
        screenWorldSpace.y = (int)floor(screenWorldSpace.y / 16);
    } else { 
        screenWorldSpace.y = (int)(screenWorldSpace.y / 16);
    }   

    return screenWorldSpace;
} 


Vector2 getChunkPosition(Vector2 gridPosition, int chunkSize)
{
    int xChunkPos = floor(gridPosition.x / chunkSize); 
    int yChunkPos = floor(gridPosition.y / chunkSize); 

    return Vector2{(float)xChunkPos, (float)yChunkPos};
}


Vector2 getChunkToGrid(Vector2 chunkPosition, int chunkSize)
{
    return Vector2{chunkPosition.x * chunkSize, chunkPosition.y * chunkSize};
}

Vector2 convertChunkToWorldSpace(Vector2 chunkPosition,  int chunkSize)
{
    return Vector2{chunkPosition.x * chunkSize * 16, chunkPosition.y * chunkSize * 16};
}
