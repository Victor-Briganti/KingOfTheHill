#ifndef __SCENE6_H__
#define __SCENE6_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene6;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE6_HEIGHT (16) /* In Tile */
#define MAP_SCENE6_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 1
#define MAP_SCENE6_X_POS (14) /* In Tile */
#define MAP_SCENE6_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE6_init();

SceneId SCENE6_update();

void SCENE6_hit(const Vect2D_s16 hitPos);

void SCENE6_destroy();

#endif // __SCENE6_H__
