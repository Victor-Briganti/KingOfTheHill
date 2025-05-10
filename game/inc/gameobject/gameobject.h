#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <genesis.h>
#include "global.h"

typedef struct GameObject {
    Sprite *sprite;
    s8 x_pos;
    s8 y_pos;
} GameObject;

inline void GAMEOBJECT_clamp(GameObject * const obj, u16 width, u16 height) {
    if (obj->x_pos < 0)
    obj->x_pos = 0;

  if (obj->x_pos >= width)
    obj->x_pos = width - 1;

  if (obj->y_pos < 0)
    obj->y_pos = 0;

  if (obj->y_pos >= height)
    obj->y_pos = height - 1;
}

inline void GAMEOBJECT_init(GameObject *const obj, const SpriteDefinition *sprite, u16 palette,
    s8 x_pos, s8 y_pos)
{
    PAL_setPalette(palette, sprite->palette->data, DMA);
    obj->sprite = SPR_addSprite(sprite, POS_X(x_pos), POS_Y(y_pos), TILE_ATTR(PLAYER_PAL, FALSE, FALSE, FALSE));
    obj->x_pos = x_pos;
    obj->y_pos = y_pos;
}

inline void GAMEOBJECT_updatePos(GameObject *const obj, u16 widthLimit, u16 heighLimit) {
  GAMEOBJECT_clamp(obj, widthLimit, heighLimit);
  SPR_setPosition(obj->sprite, POS_X(obj->x_pos), POS_Y(obj->y_pos));
}


#endif //  __GAMEOBJECT_H__
