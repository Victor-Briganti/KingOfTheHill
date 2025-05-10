#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <genesis.h>

//===----------------------------------------------------------------------===//
// SCREEN
//===----------------------------------------------------------------------===//

// Screen size
#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (224)

//===----------------------------------------------------------------------===//
// PALETTE
//===----------------------------------------------------------------------===//

// Palettes
#define PLAYER_PAL PAL0
#define ENEMY_PAL PAL1
#define TILEMAP_PAL PAL2
#define BACKGROUND_PAL PAL3

//===----------------------------------------------------------------------===//
// PLANE
//===----------------------------------------------------------------------===//

// Planes
#define BACKGROUND_PLANE BG_B
#define TILEMAP_PLANE BG_A

// Level Map Size Information
#define MAP_LEVEL1_HEIGHT (16)  /* In Tile */
#define MAP_LEVEL1_WIDTH (14)   /* In Tile */

// Initial position of the map in level 1
#define MAP_LEVEL1_X_POS (14)  /* In Tile */
#define MAP_LEVEL1_Y_POS (6)   /* In Tile */

// Transform the position of the player from tile to coodinates
#define POS_X(x) (((x) + MAP_LEVEL1_X_POS) * 8)
#define POS_Y(y) (((y) + MAP_LEVEL1_Y_POS) * 8)

// Player initial position
#define GOBLIN_LEVEL1_X_POS (6)
#define GOBLIN_LEVEL1_Y_POS (14)

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

//===----------------------------------------------------------------------===//
// TEXT
//===----------------------------------------------------------------------===//

// Score position
#define SCORE_TEXT_X 0
#define SCORE_TEXT_Y 1

// Level position
#define LEVEL_TEXT_X 15
#define LEVEL_TEXT_Y 1

#endif // __GLOBAL_H__
