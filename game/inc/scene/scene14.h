#ifndef __SCENE14_H__
#define __SCENE14_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene14;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE14_HEIGHT (16) /* In Tile */
#define MAP_SCENE14_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 12
#define MAP_SCENE14_X_POS (14) /* In Tile */
#define MAP_SCENE14_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE14_init();

SceneId SCENE14_update();

void SCENE14_hit(const Vect2D_s16 hitPos);

void SCENE14_destroy();

#endif // __SCENE14_H__