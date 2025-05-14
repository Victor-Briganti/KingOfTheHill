#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "global.h"

#include <genesis.h>

typedef struct GameObject {
  Sprite *sprite;
  s16 x;
  s16 y;
} GameObject;

inline void GAMEOBJECT_init(GameObject *const obj,
                            const SpriteDefinition *sprite, u16 palette, s16 x,
                            s16 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  obj->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                              TILE_ATTR(palette, FALSE, FALSE, FALSE));

  obj->x = x;
  obj->y = y;
}

inline void GAMEOBJECT_updatePos(GameObject *object) {
  SPR_setPosition(object->sprite, POS_X(object->x), POS_Y(object->y));
}

inline void GAMEOBJECT_hiddeSprite(GameObject *const obj) {
  SPR_setVisibility(obj->sprite, HIDDEN);
}

#endif //  __GAMEOBJECT_H__
