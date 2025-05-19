#ifndef __HEART_H__
#define __HEART_H__

#include "node/overlay.h"

#include <sprite_eng.h>
#include <sprites.h>

typedef enum HeartStatus {
  FULL = 2,
  HALF = 1,
  EMPTY = 0,
} HeartStatus;

typedef struct Heart {
  OverlayNode node[3];
  HeartStatus status[3];
} Heart;

extern Heart heart;

void HEART_init();
void HEART_update();

#endif // __HEART_H__
