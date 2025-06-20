#ifndef __END_H__
#define __END_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene end;

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void END_init();

SceneId END_update();

void END_hit(const Vect2D_s16 hitPos);

void END_destroy();

#endif // __END_H__
