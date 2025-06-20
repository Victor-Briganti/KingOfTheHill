#ifndef __CREDITS_H__
#define __CREDITS_H__

#include "scene_manager.h"

#include <genesis.h>

extern Scene credits;

//===----------------------------------------------------------------------===//
// PUBLIC
//===----------------------------------------------------------------------===//

void CREDITS_init();

SceneId CREDITS_update();

void CREDITS_hit(const Vect2D_s16 hitPos);

void CREDITS_destroy();

#endif // __CREDITS_H__
