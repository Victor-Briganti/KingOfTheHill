#ifndef __SCENE11_H__
#define __SCENE11_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene11;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE11_HEIGHT (16) /* In Tile */
#define MAP_SCENE11_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 11
#define MAP_SCENE11_X_POS (14) /* In Tile */
#define MAP_SCENE11_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE11_init();

SceneId SCENE11_update();

void SCENE11_hit(const Vect2D_s16 hitPos);

void SCENE11_destroy();

#endif // __SCENE11_H__