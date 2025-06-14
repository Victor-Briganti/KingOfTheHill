#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene gameOver;

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void GAME_OVER_init();

SceneId GAME_OVER_update();

void GAME_OVER_hit(const Vect2D_s16 hitPos);

void GAME_OVER_destroy();

#endif // __GAME_OVER_H__
