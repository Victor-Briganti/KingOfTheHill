#ifndef __INIT_H__
#define __INIT_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene init;

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void INIT_init();

SceneId INIT_update();

void INIT_hit(const Vect2D_s16 hitPos);

void INIT_destroy();

#endif // __INIT_H__
