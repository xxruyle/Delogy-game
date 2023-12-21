#ifndef MACROS_UTIL 
#define MACROS_UTIL 

#include <iostream>   
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <vector>
#include <string>
#include <cassert>

/* Window info*/
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 576  

/* Game World Info*/
#define WORLD_SIZE 3
#define CHUNK_SIZE 10
#define DATA_STRIDE 3 
#define RENDER_DISTANCE 3 // the amount of chunks near the player the tile manager will render

#endif 