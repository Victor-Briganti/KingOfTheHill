#ifndef __SCENE1_H__
#define __SCENE1_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene1;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE1_HEIGHT (16) /* In Tile */
#define MAP_SCENE1_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 1
#define MAP_SCENE1_X_POS (14) /* In Tile */
#define MAP_SCENE1_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE1_init();

SceneId SCENE1_update();

void SCENE1_hit(const Vect2D_s16 hitPos);

void SCENE1_destroy();

#endif // __SCENE1_H__
