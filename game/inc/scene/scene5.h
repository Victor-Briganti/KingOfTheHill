#ifndef __SCENE5_H__
#define __SCENE5_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene5;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE5_HEIGHT (16) /* In Tile */
#define MAP_SCENE5_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 4
#define MAP_SCENE5_X_POS (14) /* In Tile */
#define MAP_SCENE5_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE5_init();

SceneId SCENE5_update();

void SCENE5_hit(const Vect2D_s16 hitPos);

void SCENE5_destroy();

#endif //__SCENE5_H__
