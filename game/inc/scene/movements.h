#ifndef __MOVEMENTS_H__
#define __MOVEMENTS_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene movements;

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void MOVEMENTS_init();

SceneId MOVEMENTS_update();

void MOVEMENTS_hit(const Vect2D_s16 hitPos);

void MOVEMENTS_destroy();

#endif // __MOVEMENTS_H__
