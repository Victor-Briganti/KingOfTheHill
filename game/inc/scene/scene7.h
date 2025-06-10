#ifndef __SCENE7_H__
#define __SCENE7_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene7;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE7_HEIGHT (16) /* In Tile */
#define MAP_SCENE7_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 7
#define MAP_SCENE7_X_POS (14) /* In Tile */
#define MAP_SCENE7_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE7_init();

SceneId SCENE7_update();

void SCENE7_hit(const Vect2D_s16 hitPos);

void SCENE7_destroy();

#endif // __SCENE7_H__
