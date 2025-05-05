#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <genesis.h>

// Screen size
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

// Palettes
#define PLAYER_PAL PAL0
#define ENEMY_PAL PAL1
#define BACKGROUND_PAL PAL3

// Planes
#define BACKGROUND_PLANE BG_B

// Board size
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 8

// Padding of the black corners of the screen
#define PADDING_LEFT 6
#define PADDING_TOP 3

// Movement of the player or enemy
#define POS_Y(y) ((y) + PADDING_TOP) * 16
#define POS_X(x) ((x) + PADDING_LEFT) * 16

#endif // _GLOBAL_H_
