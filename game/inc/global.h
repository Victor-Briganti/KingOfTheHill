#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <genesis.h>

// Screen size
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

// Palettes
#define PLAYER_PAL PAL0
#define ENEMY_PAL PAL1
#define TILEMAP_PAL PAL2
#define BACKGROUND_PAL PAL3

// Planes
#define BACKGROUND_PLANE BG_B
#define TILEMAP_PLANE BG_A

// Position of each tile color
#define BLACK_TILE 0
#define WHITE_TILE 1
#define PURPLE_TILE 2
#define GREEN_TILE 3
#define RED_TILE 4
#define BLUE_TILE 5

// Level Map Size Information
#define MAP_LEVEL1_HEIGHT 15 /* In Tile */
#define MAP_LEVEL1_WIDTH 7   /* In Tile */

// Initial position of the map in level 1
#define MAP_LEVEL1_X_POS MAP_LEVEL1_HEIGHT /* In Tile */
#define MAP_LEVEL1_Y_POS MAP_LEVEL1_WIDTH  /* In Tile */

// Movement of the player or enemy
#define POS_X(x) ((x) + MAP_LEVEL1_X_POS) * 8
#define POS_Y(y) ((y) + MAP_LEVEL1_Y_POS) * 8


// #TODO: This definitions are only the idea of how many pieces each board
// should have 

// #define MAP_MAX_HEIGHT 12
// #define MAP_MAX_WIDTH 11

// #define MAP_LEVEL2_HEIGHT 10 #define MAP_LEVEL2_WIDTH 7

// #define MAP_LEVEL3_HEIGHT 10
// #define MAP_LEVEL3_WIDTH 9

// #define MAP_LEVEL4_HEIGHT 10
// #define MAP_LEVEL4_WIDTH 11

// #define MAP_LEVEL5_HEIGHT 12
// #define MAP_LEVEL5_WIDTH 11

// #define MAP_LEVEL6_HEIGHT 12
// #define MAP_LEVEL6_WIDTH 11

#endif // __GLOBAL_H__
