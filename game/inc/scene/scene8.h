#ifndef __SCENE8_H__
#define __SCENE8_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene scene8;

//===----------------------------------------------------------------------===//
// DEFINITIONS
//===----------------------------------------------------------------------===//

// Scene Map Size Information
#define MAP_SCENE8_HEIGHT (16) /* In Tile */
#define MAP_SCENE8_WIDTH (14)  /* In Tile */

// Initial position of the map in scene 7
#define MAP_SCENE8_X_POS (14) /* In Tile */
#define MAP_SCENE8_Y_POS (6)  /* In Tile */

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void SCENE8_init();

SceneId SCENE8_update();

void SCENE8_hit(const Vect2D_s16 hitPos);

void SCENE8_destroy();

#endif // __SCENE8_H__
