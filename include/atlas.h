#pragma once 
#include "raylib.h" 
#include <iostream> 
#include <cassert>

struct Atlas 
{
    Texture2D texture; 
    Atlas(const char* filePath);   
    ~Atlas();
};