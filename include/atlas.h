#pragma once 
#include "macros_util.h"
#include "raylib.h" 

struct Atlas 
{
    Texture2D texture; 
    Atlas(const char* filePath);   
    ~Atlas();
};