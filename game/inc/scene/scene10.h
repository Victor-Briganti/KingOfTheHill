#ifndef __SCENE10_H__
#define __SCENE10_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene10;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE10_HEIGHT (16) /* In Tile */
#define MAP_SCENE10_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 10
#define MAP_SCENE10_X_POS (14) /* In Tile */
#define MAP_SCENE10_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE10_init();

SceneId SCENE10_update();

void SCENE10_hit(const Vect2D_s16 hitPos);

void SCENE10_destroy();

#endif // __SCENE10_H__