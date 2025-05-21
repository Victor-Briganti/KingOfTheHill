#ifndef __SCENE1_H__
#define __SCENE1_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene1;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE1_HEIGHT (16) /* In Tile */
#define MAP_SCENE1_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 1
#define MAP_SCENE1_X_POS (14) /* In Tile */
#define MAP_SCENE1_Y_POS (6)  /* In Tile */

// Player initial position
#define PLAYER_SCENE1_X_POS (6)  /* In Tile */
#define PLAYER_SCENE1_Y_POS (14) /* In Tile */

// Pawn initial position
#define PAWN_SCENE1_X_POS (6) /* In Tile */
#define PAWN_SCENE1_Y_POS (0) /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE1_init();

s8 SCENE1_update();

void SCENE1_hitEnemy();

void SCENE1_destroy();

#endif // __SCENE1_H__
