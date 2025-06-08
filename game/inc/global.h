#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "scene/scene_manager.h"

#include <genesis.h>

//===----------------------------------------------------------------------===//
// EXTERN
//===----------------------------------------------------------------------===//

// Define the "frame" of the game
extern u16 frame;

// Define the tile index of the game
extern u16 tile_index;

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
#define LEVEL_TEXT_X 17
#define LEVEL_TEXT_Y 1

//===----------------------------------------------------------------------===//
// FRAME
//===----------------------------------------------------------------------===//

#define FRAME_ANIMATION 30

#endif // __GLOBAL_H__
