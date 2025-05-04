#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// Screen size
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

// Board size
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

// Padding of the black corners of the screen
#define PADDING_LEFT 6
#define PADDING_TOP 3

// Movment of the player or enemy
#define POS_Y(y) ((y) + PADDING_TOP) * 16
#define POS_X(x) ((x) + PADDING_LEFT) * 16

#endif // _GLOBAL_H_
