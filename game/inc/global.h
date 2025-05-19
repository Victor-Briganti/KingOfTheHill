#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <genesis.h>

typedef enum GameTurn {
  PLAYER = 0,
  ENEMY = 1,
  GAME_OVER = 2,
} GameTurn;

//===----------------------------------------------------------------------===//
// EXTERN
//===----------------------------------------------------------------------===//

// Current Level Size Information
extern u16 mapLevelHeight;
extern u16 mapLevelWidth;

// Current Map Level Size Information
extern u16 mapLevelX;
extern u16 mapLevelY;

// Current Level Player Initial Position
extern s16 playerInitX;
extern s16 playerInitY;

// Define who turn is this
extern GameTurn turn;

// Define the "frame" of the game
extern u16 frame;

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

// Transform the position of the player from tile to coodinates
#define POS_X(x) (((x) + mapLevelX) * 8)
#define POS_Y(y) (((y) + mapLevelY) * 8)

//===----------------------------------------------------------------------===//
// TEXT
//===----------------------------------------------------------------------===//

// Score position
#define SCORE_TEXT_X 0
#define SCORE_TEXT_Y 1

// Level position
#define LEVEL_TEXT_X 15
#define LEVEL_TEXT_Y 1

//===----------------------------------------------------------------------===//
// FRAME
//===----------------------------------------------------------------------===//

#define FRAME_ANIMATION 30

#endif // __GLOBAL_H__
