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

// Level position
#define LEVEL_TEXT_X 17
#define LEVEL_TEXT_Y 1

//===----------------------------------------------------------------------===//
// FRAME
//===----------------------------------------------------------------------===//

#define FRAME_ANIMATION 8

//===----------------------------------------------------------------------===//
// SOUND
//===----------------------------------------------------------------------===//

// ID of every sound
#define ID_MOVE_SOUND 64
#define ID_DEATH_SOUND 65
#define ID_ATTACK_SOUND 66

// Length of the array generated at sfx.h
#define LEN_MOVE_SOUND 17152
#define LEN_DEATH_SOUND 19712
#define LEN_ATTACK_SOUND 25856

// Priority of sounds
#define PRIO_MOVE_SOUND 5
#define PRIO_DEATH_SOUND 0
#define PRIO_ATTACK_SOUND 0

// Channles used by each sound
#define CHN_MOVE_SOUND SOUND_PCM_CH3
#define CHN_DEATH_SOUND SOUND_PCM_CH2
#define CHN_ATTACK_SOUND SOUND_PCM_CH2

#endif // __GLOBAL_H__
