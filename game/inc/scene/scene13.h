#ifndef __SCENE13_H__
#define __SCENE13_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene13;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE13_HEIGHT (16) /* In Tile */
#define MAP_SCENE13_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 12
#define MAP_SCENE13_X_POS (14) /* In Tile */
#define MAP_SCENE13_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE13_init();

SceneId SCENE13_update();

void SCENE13_hit(const Vect2D_s16 hitPos);

void SCENE13_destroy();

#endif // __SCENE13_H__