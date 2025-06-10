#ifndef __SCENE15_H__
#define __SCENE15_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene15;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE15_HEIGHT (16) /* In Tile */
#define MAP_SCENE15_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 12
#define MAP_SCENE15_X_POS (14) /* In Tile */
#define MAP_SCENE15_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE15_init();

SceneId SCENE15_update();

void SCENE15_hit(const Vect2D_s16 hitPos);

void SCENE15_destroy();

#endif // __SCENE15_H__