#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "global.h"

#include <genesis.h>

typedef struct GameObject {
  Sprite *sprite;
  s16 x;
  s16 y;
} GameObject;

inline void GAMEOBJECT_initInBoard(GameObject *const object,
                                   const SpriteDefinition *sprite, u16 palette,
                                   s16 x, s16 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  object->sprite = SPR_addSprite(sprite, POS_X(x), POS_Y(y),
                                 TILE_ATTR(palette, FALSE, FALSE, FALSE));

  object->x = x;
  object->y = y;
}

inline void GAMEOBJECT_initRawCoords(GameObject *const object,
                                     const SpriteDefinition *sprite,
                                     u16 palette, s16 x, s16 y) {
  PAL_setPalette(palette, sprite->palette->data, DMA);
  object->sprite =
      SPR_addSprite(sprite, x, y, TILE_ATTR(palette, FALSE, FALSE, FALSE));

  object->x = x;
  object->y = y;
}

inline void GAMEOBJECT_updatePos(GameObject *object) {
  SPR_setPosition(object->sprite, POS_X(object->x), POS_Y(object->y));
}

inline void GAMEOBJECT_releaseSprite(GameObject *const object) {
  SPR_releaseSprite(object->sprite);
}

inline bool GAMEOBJECT_animateTo(GameObject *const object, s16 posX, s16 posY) {
  if (object->x < posX)
    object->x++;
  else if (object->x > posX)
    object->x--;

  if (object->y < posY)
    object->y++;
  else if (object->y > posY)
    object->y--;

  GAMEOBJECT_updatePos(object);

  return (object->x == posX && object->y == posY);
}

#endif //  __GAMEOBJECT_H__
