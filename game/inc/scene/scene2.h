#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene2;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE2_HEIGHT (16) /* In Tile */
#define MAP_SCENE2_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 2
#define MAP_SCENE2_X_POS (14) /* In Tile */
#define MAP_SCENE2_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE2_init();

SceneId SCENE2_update();

void SCENE2_hitEnemy(const Vect2D_s16 hitPos);

void SCENE2_destroy();

#endif // __SCENE2_H__
