#ifndef __SCENE3_H__
#define __SCENE3_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene3;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE3_HEIGHT (16) /* In Tile */
#define MAP_SCENE3_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 3
#define MAP_SCENE3_X_POS (14) /* In Tile */
#define MAP_SCENE3_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE3_init();

SceneId SCENE3_update();

void SCENE3_hit(const Vect2D_s16 hitPos);

void SCENE3_destroy();

#endif //__SCENE3_H__
