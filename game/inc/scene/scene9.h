#ifndef __SCENE9_H__
#define __SCENE9_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene9;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE9_HEIGHT (16) /* In Tile */
#define MAP_SCENE9_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 9
#define MAP_SCENE9_X_POS (14) /* In Tile */
#define MAP_SCENE9_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE9_init();

SceneId SCENE9_update();

void SCENE9_hit(const Vect2D_s16 hitPos);

void SCENE9_destroy();

#endif // __SCENE9_H__