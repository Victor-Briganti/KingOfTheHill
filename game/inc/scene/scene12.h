#ifndef __SCENE12_H__
#define __SCENE12_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene12;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE12_HEIGHT (16) /* In Tile */
#define MAP_SCENE12_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 12
#define MAP_SCENE12_X_POS (14) /* In Tile */
#define MAP_SCENE12_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE12_init();

SceneId SCENE12_update();

void SCENE12_hit(const Vect2D_s16 hitPos);

void SCENE12_destroy();

#endif // __SCENE12_H__