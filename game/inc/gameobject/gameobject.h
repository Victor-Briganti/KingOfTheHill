#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "global.h"
#include <genesis.h>

typedef struct GameObject {
  Sprite *sprite;
  s8 x;
  s8 y;
} GameObject;

inline void GAMEOBJECT_clamp(GameObject *const obj, u16 width, u16 height) {
  if (obj->x < 0)
    obj->x = 0;

  if (obj->x >= width)
    obj->x = width - 1;

  if (obj->y < 0)
    obj->y = 0;

  if (obj->y >= height)
    obj->y = height - 1;
}

inline void GAMEOBJECT_init(GameObject *const obj,
                            const SpriteDefinition *sprite, u16 palette, s8 x,
                            s8 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  obj->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                              TILE_ATTR(PLAYER_PAL, FALSE, FALSE, FALSE));
  obj->x = x;
  obj->y = y;
}

inline void GAMEOBJECT_updatePos(GameObject *const obj, u16 width, u16 height) {
  GAMEOBJECT_clamp(obj, width, height);
  SPR_setPosition(obj->sprite, POS_X(obj->x), POS_Y(obj->y));
}

#endif //  __GAMEOBJECT_H__
