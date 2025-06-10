#ifndef __SCENE4_H__
#define __SCENE4_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene4;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE4_HEIGHT (16) /* In Tile */
#define MAP_SCENE4_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 4
#define MAP_SCENE4_X_POS (14) /* In Tile */
#define MAP_SCENE4_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE4_init();

SceneId SCENE4_update();

void SCENE4_hit(const Vect2D_s16 hitPos);

void SCENE4_destroy();

#endif //__SCENE4_H__
